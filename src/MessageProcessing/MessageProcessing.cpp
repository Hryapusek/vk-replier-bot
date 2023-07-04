#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>
#include "MessageProcessing.hpp"
#include "../ConfigReader.hpp"
#include "../VkApi/Requests/Messages/MessagesSendRequest.hpp"
#include "../VkApi/Exceptions.hpp"
#include "../BotTypes/Chats.hpp"

namespace
{
  using namespace commands;
  using namespace vk::requests::messages;
  using namespace vk::objects;
  using namespace vk::callback::event::objects;
  using str_cref = const std::string &;

  /// @return True if EOL or EOF
  bool skipWord(std::string::const_iterator &beg, str_cref text)
  {
    while (beg != text.cend() && !isspace(*beg))
      ++beg;
    while (beg != text.cend() && (*beg == ' ' || *beg == '\t'))
      ++beg;
    return !(beg == text.cend() || *beg == '\n');
  }

  /// @throw \b std::logic_error if string is not closed with -> "
  std::string extractString(std::string::const_iterator &beg, str_cref text)
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
  void extractNumAndTitle(str_cref text, size_t pos, std::optional< int > &num, std::string &title)
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

  /// @return Empty string if no quotes were found right after command.
  /// Quoted title otherwise.
  /// @throw \b std::logic_error if unclosed quote found
  std::string extractTitle(str_cref text, size_t pos)
  {
    std::string title;
    auto beg = std::string::const_iterator(&text[pos]);
    if (!skipWord(beg, text) || *beg != '"')
      return title;
    else
      return extractString(beg, text);
  }

  void logAndSendErrorMessage(MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    BOOST_LOG_TRIVIAL(warning) << command << ": " << errorMessage << ". Skipping message";
    req.message(errorMessage).execute();
  }

  bool checkIfCommandNotFromChat(const Message &message, str_cref command)
  {
    if (!message.fromChat())
    {
      BOOST_LOG_TRIVIAL(warning) << "Command  " << command << " not from chat. Skipping message";
      return false;
    }
    return true;
  }

  bool checkIfChatIsSource(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    auto sourceChat = config::ConfigHolder::getReadOnlyConfig().config.sourceChat;
    if (sourceChat && sourceChat.value().peer_id == message.peer_id.value())
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  bool checkIfChatPresentInTable(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    if (config::ConfigHolder::getReadOnlyConfig().config.targetsTable.containsPeerId(message.peer_id.value()))
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  namespace reg_target_helpers
  {
    bool checkIfNumberBusy(int num, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
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
    static const char *command = "regTarget";
    if (!checkIfCommandNotFromChat(message, command))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.peer_id.value());
    if (!checkIfChatIsSource(message, req, command, "This chat is source. Can not register") ||
        !checkIfChatPresentInTable(message, req, command, "This chat is present somewhere in the table"))
      return;
    std::optional< int > numOpt;
    std::string title;
    extractNumAndTitle(message.text.value(), pos, numOpt, title);
    if (numOpt)
    {
      if (!checkIfNumberBusy(numOpt.value(), req, command, "Given num is already busy"))
        return;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(numOpt.value(), TargetChat{ numOpt.value(), message.peer_id.value(), "title" });
      if (!res)
      {
        logAndSendErrorMessage(req, command, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    else
    {
      TargetChat chat = TargetChat();
      chat.peer_id = message.peer_id.value();
      chat.title = title;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(std::move(chat));
      if (!res)
      {
        logAndSendErrorMessage(req, command, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    BOOST_LOG_TRIVIAL(info) << "Successfully registered target " << message.peer_id.value();
    req.message("Successfully registered as target!").execute();
    return;
  }

  namespace reg_source_helpers
  {
    bool checkIfSourceChatNotEmpty(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
    {
      if (config::ConfigHolder::getReadOnlyConfig().config.sourceChat)
      {
        logAndSendErrorMessage(req, command, errorMessage);
        return false;
      }
      return true;
    }
  }

  void regSource(const Message &message, size_t pos)
  {
    using namespace config;
    using namespace reg_source_helpers;
    static const char *command = "regSource";
    if (!checkIfCommandNotFromChat(message, command))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.peer_id.value());
    if (!checkIfSourceChatNotEmpty(message, req, command, "Delete current sourceChat first") ||
        !checkIfChatIsSource(message, req, command, "This chat is already source") ||
        !checkIfChatPresentInTable(message, req, command, "This chat is present somewhere in the table"))
      return;
    std::string title = extractTitle(message.text.value(), pos);
    ConfigHolder::getReadWriteConfig().config.sourceChat = SourceChat{ message.peer_id.value(), title };
    BOOST_LOG_TRIVIAL(info) << "Successfully registered source " << message.peer_id.value();
    req.message("Successfully registered as source!").execute();
    return;
  }

  namespace reg_checker_helpers
  {
    bool checkIfAlreadyChecker(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
    {
      auto configWrap = config::ConfigHolder::getReadOnlyConfig();
      auto checkersOpt = configWrap.config.statusCheckersIds;
      if (checkersOpt)
      {
        auto it = std::find(checkersOpt.value().begin(), checkersOpt.value().end(), message.peer_id.value());
        if (it != checkersOpt.value().end())
        {
          logAndSendErrorMessage(req, command, errorMessage);
          return false;
        }
      }
      return true;
    }
  }

  void regChecker(const Message &message, size_t pos)
  {
    using namespace config;
    using namespace reg_checker_helpers;
    static const char *command = "regChecker";
    if (!message.fromDirect())
    {
      BOOST_LOG_TRIVIAL(warning) << "regChecker found BUT not in direct";
      return;
    }
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.peer_id.value());
    if (!checkIfAlreadyChecker(message, req, command, "User is already checker"))
      return;
    ConfigHolder::getReadWriteConfig().config.statusCheckersIds.value().push_back(message.peer_id.value());
    BOOST_LOG_TRIVIAL(info) << "Successfully registered checker " << message.peer_id.value();
    req.message("Successfully registered checker!").execute();
    return;
  }

  void sendMessageToAllTargets(str_cref title, int fwd_msg_id)
  {
    MessagesSendRequest req;
    req.peer_ids(config::ConfigHolder::getTargetIds())
    .random_id(0)
    .message(title)
    .forward_messages(std::to_string(fwd_msg_id))
    .execute();
  }

  Tag findTag(str_cref text, size_t &pos)
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

  // TODO optimize by searching '/' first
  Command findCommand(str_cref text, size_t &pos)
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
    if (message.peer_id.value() != config::ConfigHolder::getSourceChat().peer_id)
    {
      BOOST_LOG_TRIVIAL(info) << "Skipping new message - not from source\n";
      return;
    }
    size_t pos = 0;
    Tag tag = findTag(message.text.value(), pos);
    switch (tag)
    {
    case Tag::ALL:
    {
      try
      {
        BOOST_LOG_TRIVIAL(info) << "Found Tag::ALL(without @all)\n";
        std::string title = extractTitle(message.text.value(), pos);
        sendMessageToAllTargets(std::move(title), message.id.value());
      }
      catch (const std::logic_error &e)
      {
        MessagesSendRequest()
        .peer_id(message.peer_id.value())
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
        std::string title = "@all, " + extractTitle(message.text.value(), pos);
        sendMessageToAllTargets(std::move(title), message.id.value());
        break;
      }
      catch (const std::logic_error &e)
      {
        MessagesSendRequest()
        .peer_id(message.peer_id.value())
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
    Command cmd = findCommand(message.text.value(), pos);
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
    case Command::REG_CHECKER:
    {
      BOOST_LOG_TRIVIAL(info) << "REG_CHECKER command found";
      regChecker(message, pos);
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
  void sendErrorMessage(const std::exception &e, int peer_id)
  {
    try
    {
      MessagesSendRequest req;
      req.random_id(0).peer_id(peer_id).message(e.what()).execute();
    } catch (const std::exception &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Error while sending exception message";
    }
  }

  void processMessage(std::shared_ptr< NewMessage > newMessage)
  {
    using namespace config;
    if (!newMessage->getMessage().text)
    {
      BOOST_LOG_TRIVIAL(info) << "Message does not contain any text. Skipping";
      return;
    }
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
      sendErrorMessage(e, newMessage->getMessage().peer_id.value());
    }
    catch (const std::logic_error &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected logic_error was thrown. Handling current event aborted.\n" << e.what();
      sendErrorMessage(e, newMessage->getMessage().peer_id.value());
    }
    catch (const vk::exceptions::RequestException &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected RequestException was thrown. Handling current event aborted.\n" << e.what();
      sendErrorMessage(e, newMessage->getMessage().peer_id.value());
    }
    catch (const std::exception &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Unexpected std::exception was thrown. Handling current event aborted.\n" << e.what();
      sendErrorMessage(e, newMessage->getMessage().peer_id.value());
    }
  }
}
