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

  namespace reg_target_helpers
  {
    bool checkIfCommandNotFromChat(const Message &message)
    {
      if (!message.fromChat())
      {
        BOOST_LOG_TRIVIAL(warning) << "Command regTarget not from chat. Skipping message";
        return false;
      }
      return true;
    }

    bool checkIfChatSource(const Message &message, vk::MessagesSendRequest &req)
    {
      if (config::ConfigHolder::getReadOnlyConfig().config.sourceChatId == message.peer_id)
      {
        BOOST_LOG_TRIVIAL(warning) << "regTarget: Given chat is source. Skipping message";
        req.message("Given chat is source.").execute();
        return false;
      }
      return true;
    }

    bool checkIfChatPresentInTable(const Message &message, vk::MessagesSendRequest &req)
    {
      if (config::ConfigHolder::getReadOnlyConfig().config.targetsTable.containsTarget(Target{ message.peer_id, "" }))
      {
        BOOST_LOG_TRIVIAL(warning) << "regTarget: Given chat is present somewhere in the table. Skipping message";
        req.message("Given chat is present somewhere in the table.").execute();
        return false;
      }
      return true;
    }

    bool checkIfNumberBusy(vk::MessagesSendRequest &req, int num)
    {
      try
      {
        config::ConfigHolder::getReadOnlyConfig().config.targetsTable.at(num);
        BOOST_LOG_TRIVIAL(warning) << "regTarget: Given num is already busy. Skipping message";
        req.message("Given num is already busy.").execute();
        return false;
      }
      catch (const std::exception &e)
      {
        return true;
      }
    }
  }

  void regTarget(const Message &message, size_t pos)
  {
    using namespace config;
    using namespace reg_target_helpers;
    if (!checkIfCommandNotFromChat(message))
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

  /// @throw \b std::logic_error if unclosed quote found
  std::string getTitle(const std::string &text, size_t pos)
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
      //try catch if bad title
      try
      {
        BOOST_LOG_TRIVIAL(info) << "Found Tag::ALL(without @all)\n";
        std::string title = getTitle(message.text, pos);
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
      BOOST_LOG_TRIVIAL(info) << "Found Tag::ALL_IMPORTANT(with @all)\n";
      std::string title = "@all, " + getTitle(message.text, pos);
      sendMessageToAllTargets(std::move(title), message.id);
      break;
    }

    case Tag::NONE:
      BOOST_LOG_TRIVIAL(info) << "No tag found in message. Skipping";
      break;

    default:
      BOOST_LOG_TRIVIAL(error) << "Incorrect tag found. Can not imagine how. Skipping";
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
    default:
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
