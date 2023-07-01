#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>
#include "MessageProcessing.hpp"
#include "../ConfigReader.hpp"
#include "../ApiRequests/MessagesSendRequest.hpp"
#include "../ApiRequests/Exceptions.hpp"

namespace
{
  using namespace commands;

  /// @return True if EOL or EOF
  bool skipWord(std::string::const_iterator &beg, const std::string &text)
  {
    while (beg != text.cend() && !isspace(*beg))
      ++beg;
    while (beg != text.cend() && (*beg == ' ' || *beg == '\t'))
      ++beg;
    return !(beg == text.cend() || *beg == '\n');
  }

  /// @throw \b std::logic_error if string is not closed with -> "
  std::string extractString(std::string::const_iterator &beg, const std::string &text)
  {
    if (*beg == '"')
      ++beg;
    std::string result;
    while (beg != text.cend() && *beg != '"')
    {
      result += *beg;
    }
    if (beg == text.cend() || *beg != '"')
      throw std::logic_error("");
    return result;
  }

  /// @throws \b std::out_of_range, if number is incorrect.
  /// @throws \b std::invalid_argument if number is incorrect.
  /// @throws \b std::logic_error if command format is incorrect.
  void extractNumAndTitle(const std::string &text, size_t pos, std::optional< int > &num, std::string &title)
  {
    title = "";
    std::string::const_iterator beg(&text[pos]);
    if (!skipWord(beg, text))
      return;
    if (*beg == '"')
    {
      title = extractString(beg, text);
      return;
    }
    num = std::stoi(beg.base());
    if (!skipWord(beg, text))
      return;
    if (*beg == '"')
    {
      title = extractString(beg, text);
      return;
    }
    else
      throw std::logic_error("");
  }

  void logAndSendErrorMessage(vk::MessagesSendRequest &req, std::string command, std::string errorMessage)
  {
    BOOST_LOG_TRIVIAL(warning) << command << ": " << errorMessage << ". Skipping message";
    req.message(errorMessage).execute();
  }

  bool checkIfCommandNotFromChat(const Message &message, std::string command)
  {
    if (!message.fromChat())
    {
      BOOST_LOG_TRIVIAL(warning) << "Command  " << command << " not from chat. Skipping message";
      return false;
    }
    return true;
  }

  bool checkIfChatSource(const Message &message, vk::MessagesSendRequest &req, std::string command, std::string errorMessage)
  {
    if (config::ConfigHolder::getReadOnlyConfig().config.sourceChatId == message.peer_id)
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  bool checkIfChatPresentInTable(const Message &message, vk::MessagesSendRequest &req, std::string command, std::string errorMessage)
  {
    if (config::ConfigHolder::getReadOnlyConfig().config.targetsTable.containsTarget(Target{ message.peer_id, "" }))
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  namespace reg_target_helpers
  {
    bool checkIfNumberBusy(int num, vk::MessagesSendRequest &req, std::string command, std::string errorMessage)
    {
      try
      {
        config::ConfigHolder::getReadOnlyConfig().config.targetsTable.at(num);
        logAndSendErrorMessage(req, command, errorMessage);
        return false;
      }
      catch (const std::out_of_range &e)
      {
        return true;
      }
    }
  }

  void regTarget(const Message &message, size_t pos)
  {
    using namespace config;
    using namespace reg_target_helpers;
    const char *command = "regTarget";
    if (!checkIfCommandNotFromChat(message, command))
      return;
    vk::MessagesSendRequest req;
    req.random_id(0).peer_id(message.peer_id);
    if (!checkIfChatSource(message, req, command, "This chat is source. Can not register") ||
        !checkIfChatPresentInTable(message, req, command, "This chat is present somewhere in the table"))
      return;
    std::optional< int > numOpt;
    std::string title;
    extractNumAndTitle(message.text, pos, numOpt, title);
    if (numOpt)
    {
      if (!checkIfNumberBusy(numOpt.value(), req, command, "Given num is already busy"))
        return;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(numOpt.value(), Target{ message.peer_id, "title" });
      if (!res)
      {
        logAndSendErrorMessage(req, command, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    else
    {
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(Target{ message.peer_id, "title" });
      if (!res)
      {
        logAndSendErrorMessage(req, command, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    BOOST_LOG_TRIVIAL(info) << "Successfully registered target " << message.peer_id;
    req.message("Successfully registered!").execute();
    return;
  }

  namespace reg_source_helpers
  { }

  void regSource(const Message &message, size_t pos)
  {
    using namespace config;
    using namespace reg_target_helpers;
    if (!checkIfCommandNotFromChat(message, "regSource"))
      return;
    vk::MessagesSendRequest req;
    req.random_id(0).peer_id(message.peer_id);
    if (!checkIfChatSource(message, req) || !checkIfChatPresentInTable(message, req))
      return;
    std::optional< int > numOpt;
    std::string title;
    extractNumAndTitle(message.text, pos, numOpt, title);
    if (numOpt)
    {
      if (!checkIfNumberBusy(req, numOpt.value()))
        return;
      ConfigHolder::getReadWriteConfig().config.targetsTable.insert(numOpt.value(), Target{ message.peer_id, "title" });
    }
    else
    {
      ConfigHolder::getReadWriteConfig().config.targetsTable.insert(Target{ message.peer_id, "title" });
    }
    BOOST_LOG_TRIVIAL(info) << "Successfully registered target " << message.peer_id;
    req.message("Successfully registered!").execute();
    return;
  }


  void sendMessageToAllTargets(std::string &&title, int fwd_msg_id)
  {
    vk::MessagesSendRequest req;
    req.peer_ids(config::ConfigHolder::getTargetIds())
    .random_id(0)
    .message(std::move(title))
    .forward_messages(std::to_string(fwd_msg_id))
    .execute();
  }

  /// @return Empty string if no quotes were found right after command.
  /// Quoted title otherwise.
  /// @throw \b std::logic_error if unclosed quote found
  std::string extractTitle(const std::string &text, size_t pos)
  {
    std::string title;
    auto beg = std::string::const_iterator(&text[pos]);
    if (!skipWord(beg, text) || *beg != '"')
      return title;
    else
      return extractString(beg, text);
  }

  Tag findTag(const std::string &text, size_t &pos)
  {
    for (const auto & [tag, strings_to_find] : tagStrings)
      for (const auto &str : strings_to_find)
      {
        pos = text.find(str);
        if (pos != std::string::npos)
          return tag;
      }
    return Tag::NONE;
  }

  Command findCommand(const std::string &text, size_t &pos)
  {
    for (const auto & [cmd, strings_to_find] : commandStrings)
      for (const auto &str : strings_to_find)
      {
        pos = text.find(str);
        if (pos != std::string::npos)
          return cmd;
      }
    return Command::NONE;
  }

  void processMessageWithTag(const Message &message)
  {
    if (message.peer_id != config::ConfigHolder::getSourceChatId())
    {
      BOOST_LOG_TRIVIAL(info) << "Skipping new message - not from source\n";
      return;
    }
    size_t pos = 0;
    Tag tag = findTag(message.text, pos);
    switch (tag)
    {
    case Tag::ALL:
    {
      try
      {
        BOOST_LOG_TRIVIAL(info) << "Found Tag::ALL(without @all)\n";
        std::string title = extractTitle(message.text, pos);
        sendMessageToAllTargets(std::move(title), message.id);
      }
      catch (const std::logic_error &e)
      {
        vk::MessagesSendRequest()
        .peer_id(message.peer_id)
        .random_id(0)
        .message("Bad title found. Check unclosed quotes")
        .execute();
      }
      break;
    }

    case Tag::ALL_IMPORTANT:
    {
      try
      {
        BOOST_LOG_TRIVIAL(info) << "Found Tag::ALL_IMPORTANT(with @all)\n";
        std::string title = "@all, " + extractTitle(message.text, pos);
        sendMessageToAllTargets(std::move(title), message.id);
        break;
      }
      catch (const std::logic_error &e)
      {
        vk::MessagesSendRequest()
        .peer_id(message.peer_id)
        .random_id(0)
        .message("Bad title found. Check unclosed quotes")
        .execute();
      }
    }

    case Tag::NONE:
      BOOST_LOG_TRIVIAL(info) << "No tag found in message. Skipping";
      break;

    default:
      BOOST_LOG_TRIVIAL(error) << "Tag variable was not initialized! Skipping";
      break;
    }
  }

  void processMessageWithCommand(const Message &message)
  {
    size_t pos = 0;
    Command cmd = findCommand(message.text, pos);
    switch (cmd)
    {
    case Command::REG_TARGET:
    {
      BOOST_LOG_TRIVIAL(info) << "REG_TARGET command found";
      regTarget(message, pos);
      break;
    }
    case Command::REG_SOURCE:
    {
      BOOST_LOG_TRIVIAL(info) << "REG_SOURCE command found";
      regSource(message, pos);
      break;
    }
    case Command::NONE:
    {
      BOOST_LOG_TRIVIAL(info) << "No command found. Skipping";
      break;
    }
    default:
      BOOST_LOG_TRIVIAL(error) << "Command variable was not initialized! Skipping";
      break;
    }
  }
}

namespace commands
{
  void processMessage(std::shared_ptr< NewMessage > newMessage)
  {
    using namespace config;
    Mode mode = ConfigHolder::getMode();
    try
    {
      if (mode == Mode::WORK)
        processMessageWithTag(newMessage->getMessage());
      else if (mode == Mode::CONFIG)
        processMessageWithCommand(newMessage->getMessage());
    }
    catch (const Json::Exception &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected Json exception was thrown. Handling current event aborted.\n" << e.what();
    }
    catch (const std::logic_error &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected logic_error was thrown. Handling current event aborted.\n" << e.what();
    }
    catch (const vk::exceptions::RequestException &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected logic_error was thrown. Handling current event aborted.\n" << e.what();
    }
    catch (const std::exception &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected std::exception was thrown. Handling current event aborted.\n" << e.what();
    }
  }
}
