#include "MessageProcessing.hpp"

#include <boost/log/trivial.hpp>

#include "../ConfigReader.hpp"
#include "../VkApi/Requests/Messages/MessagesSendRequest.hpp"
#include "../VkApi/Exceptions.hpp"
#include "../BotTypes/Chats.hpp"

// TODO shorten title
// TODO join command and tag processing
// TODO check groupID if message from this bot(infinite cycle message sending)
// TODO add /help
// TODO print current message in logs

namespace
{
  using namespace commands;
  using namespace vk::requests::messages;
  using namespace vk::objects;
  using namespace vk::callback::event::objects;
  using str_cref = const std::string &;

  /// @note id, from_id, peer_id and text should be in message 
  void logMessage(const vk::objects::Message &msg)
  {
    BOOST_LOG_TRIVIAL(info) << "Current message:\n" 
    << "id: " << msg.getId() << '\n'
    << "from_id: " << msg.getFromId() << '\n'
    << "peer_id: " << msg.getPeerId() << '\n'
    << "text: " << msg.getText();
  }

  bool hasMessageNecessaryFields(const vk::objects::Message &m)
  {
    return m.hasFromId() && m.hasId() && m.hasPeerId() && m.hasText();
  }

  void logAndSendErrorMessage(const std::exception &e, int peer_id, str_cref exceptionName)
  {
    BOOST_LOG_TRIVIAL(error) << "Unexpected " << exceptionName << " was thrown. Handling current event aborted.\n" << e.what();
    try
    {
      MessagesSendRequest req;
      req.random_id(0).peer_id(peer_id).message(e.what()).execute();
    } catch (const std::exception &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Error while sending exception message";
    }
  }

  /// @return True if not EOL or EOF
  bool skipWord(std::string::const_iterator &beg, str_cref text)
  {
    while (beg != text.cend() && !isspace(*beg))
      ++beg;
    while (beg != text.cend() && (*beg == ' ' || *beg == '\t'))
      ++beg;
    return !(beg == text.cend() || *beg == '\n');
  }

  /// @throw \b std::exception if string is not closed with -> "
  std::string extractString(std::string::const_iterator &beg, str_cref text)
  {
    if (*beg == '"')
      ++beg;
    std::string result;
    while (beg != text.cend() && *beg != '"')
    {
      result += *beg;
      ++beg;
    }
    if (beg == text.cend())
      throw std::exception();
    return result;
  }

  /// @throws \b std::out_of_range, if number is incorrect.
  /// @throws \b std::invalid_argument if number is incorrect.
  /// @throws \b std::exception if command format is incorrect.
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
      throw std::exception();
  }

  /// @return Empty string if no quotes were found right after command.
  /// Quoted title otherwise.
  /// @throw \b std::exception if unclosed quote found
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
    if (sourceChat && sourceChat.value().peer_id == message.getPeerId())
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  bool checkIfChatPresentInTable(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    if (config::ConfigHolder::getReadOnlyConfig().config.targetsTable.containsPeerId(message.getPeerId()))
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
    static str_cref command = "regTarget";
    if (!checkIfCommandNotFromChat(message, command))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfChatIsSource(message, req, command, "This chat is source. Can not register") ||
        !checkIfChatPresentInTable(message, req, command, "This chat is present somewhere in the table"))
      return;
    std::optional< int > numOpt;
    std::string title;
    try
    {
      extractNumAndTitle(message.getText(), pos, numOpt, title);
    }
    catch (const std::logic_error &e)
    {
      logAndSendErrorMessage(req, command, "Incorrect number passed");
      return;
    }
    catch (const std::exception &e)
    {
      logAndSendErrorMessage(req, command, "Unclosed \" found");
      return;
    }
    if (numOpt)
    {
      if (!checkIfNumberBusy(numOpt.value(), req, command, "Given num is already busy"))
        return;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(numOpt.value(), TargetChat{ numOpt.value(), message.getPeerId(), title });
      if (!res)
      {
        logAndSendErrorMessage(req, command, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    else
    {
      TargetChat chat = TargetChat();
      chat.peer_id = message.getPeerId();
      chat.title = title;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(std::move(chat));
      if (!res)
      {
        logAndSendErrorMessage(req, command, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    BOOST_LOG_TRIVIAL(info) << command << ": Successfully registered target " << message.getPeerId();
    req.message("Successfully registered as target!").execute();
    return;
  }

  namespace reg_source_helpers
  {
    bool checkIfSourceChatNotEmpty(MessagesSendRequest &req, str_cref command, str_cref errorMessage)
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
    static str_cref command = "regSource";
    if (!checkIfCommandNotFromChat(message, command))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfSourceChatNotEmpty(req, command, "Delete current sourceChat first") ||
        !checkIfChatIsSource(message, req, command, "This chat is already source") ||
        !checkIfChatPresentInTable(message, req, command, "This chat is present somewhere in the table"))
      return;
    std::string title = extractTitle(message.getText(), pos);
    ConfigHolder::getReadWriteConfig().config.sourceChat = SourceChat{ message.getPeerId(), title };
    BOOST_LOG_TRIVIAL(info) << command << ": Successfully registered source " << message.getPeerId();
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
        auto it = std::find(checkersOpt.value().begin(), checkersOpt.value().end(), message.getPeerId());
        if (it != checkersOpt.value().end())
        {
          logAndSendErrorMessage(req, command, errorMessage);
          return false;
        }
      }
      return true;
    }
  }

  void regChecker(const Message &message)
  {
    using namespace config;
    using namespace reg_checker_helpers;
    static str_cref command = "regChecker";
    if (!message.fromDirect())
    {
      BOOST_LOG_TRIVIAL(warning) << "regChecker found BUT not in direct";
      return;
    }
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfAlreadyChecker(message, req, command, "User is already checker"))
      return;
    ConfigHolder::getReadWriteConfig().config.statusCheckersIds.value().push_back(message.getPeerId());
    BOOST_LOG_TRIVIAL(info) << command << ": Successfully registered checker " << message.getPeerId();
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

  bool checkIfSourceChatPresent(str_cref command, str_cref errorMessage)
  {
    if (!config::ConfigHolder::getReadOnlyConfig().config.sourceChat)
    {
      BOOST_LOG_TRIVIAL(warning) << command << ": " << errorMessage;
      return false;
    }
    return true;
  }

  bool checkIfChatIsSource(int peerId, str_cref command, str_cref errorMessage)
  {
    auto sourceChat = config::ConfigHolder::getReadOnlyConfig().config.sourceChat;
    if (sourceChat && sourceChat.value().peer_id == peerId)
    {
      BOOST_LOG_TRIVIAL(warning) << command << ": " << errorMessage;
      return false;
    }
    return true;
  }

  bool checkMode(config::Mode mode, str_cref errorMessage)
  {
    if (config::ConfigHolder::getMode() != mode)
    {
      std::string postfix;
      switch (mode)
      {
        case config::Mode::CONFIG:
          postfix = " - mode is not CONFIG";
          break;

        case config::Mode::WORK:
          postfix = " - mode is not WORK";
          break;
      }
      BOOST_LOG_TRIVIAL(warning) << errorMessage << postfix;
      return false;
    }
    return true;
  }

  void tagAll(const Message &message, size_t pos)
  {
    std::string title;
    try
    {
      BOOST_LOG_TRIVIAL(info) << "Extracting title";
      title = extractTitle(message.getText(), pos);
    }
    catch (const std::exception &e)
    {
      MessagesSendRequest()
      .peer_id(message.getPeerId())
      .random_id(0)
      .message("Bad title found. After tag should be a new line or a quoted sentence. Make sure that quotes are closed")
      .execute();
      return;
    }
    BOOST_LOG_TRIVIAL(info) << "Forwarding messages";
    sendMessageToAllTargets(std::move(title), message.getId());
  }

  void tagAllImportant(const Message &message, size_t pos)
  {
    if (!checkMode(config::Mode::WORK, "Skipping TAG::ALL_IMPORTANT"))
      return;
    std::string title;
    try
    {
      BOOST_LOG_TRIVIAL(info) << "Extracting title";
      title = "@all, " + extractTitle(message.getText(), pos);
    }
    catch (const std::exception &e)
    {
      MessagesSendRequest()
      .peer_id(message.getPeerId())
      .random_id(0)
      .message("Bad title found. After tag should be a new line or a quoted sentence. Make sure that quotes are closed")
      .execute();
      return;
    }
    BOOST_LOG_TRIVIAL(info) << "Forwarding messages";
    sendMessageToAllTargets(std::move(title), message.getId());
  }

  void processMessageWithTag(const Message &message, Tag tag, size_t pos)
  {
    switch (tag)
    {
      case Tag::ALL:
      {
        static const std::string tagName = "TAG:ALL";
        BOOST_LOG_TRIVIAL(info) << "Message with " << tagName;
        if (!checkIfSourceChatPresent(tagName, "Source chat not registered. Skipping message") ||
            !checkIfChatIsSource(message.getPeerId(), tagName, "Tag used outside the source chat. Skipping message") ||
            !checkMode(config::Mode::WORK, "Skipping message with " + tagName))
          return;
        tagAll(message, pos);
        break;
      }

      case Tag::ALL_IMPORTANT:
      {
        static const std::string tagName = "Tag::ALL_IMPORTANT";
        BOOST_LOG_TRIVIAL(info) << "Message with " << tagName;
        if (!checkIfSourceChatPresent(tagName, "Source chat not registered. Skipping message") ||
            !checkIfChatIsSource(message.getPeerId(), tagName, "Tag used outside the source chat. Skipping message") ||
            !checkMode(config::Mode::WORK, "Skipping message with " + tagName))
          return;
        tagAllImportant(message, pos);
        break;
      }

      case Tag::NONE:
        BOOST_LOG_TRIVIAL(warning) << "TAG::NONE in processMessageWithTag. Skipping";
        break;

      default:
        BOOST_LOG_TRIVIAL(warning) << "Tag variable was not initialized in processMessageWithTag. Skipping";
        break;
    }
  }

  void processMessageWithCommand(const Message &message, Command cmd, size_t pos)
  {
    switch (cmd)
    {
      case Command::REG_TARGET:
      {
        BOOST_LOG_TRIVIAL(info) << "REG_TARGET command found";
        checkMode(config::Mode::CONFIG, "Can not use this command");
        regTarget(message, pos);
        break;
      }
      case Command::REG_SOURCE:
      {
        BOOST_LOG_TRIVIAL(info) << "REG_SOURCE command found";
        checkMode(config::Mode::CONFIG, "Can not use this command");
        regSource(message, pos);
        break;
      }
      case Command::REG_CHECKER:
      {
        BOOST_LOG_TRIVIAL(info) << "REG_CHECKER command found";
        checkMode(config::Mode::CONFIG, "Can not use this command");
        regChecker(message);
        break;
      }
      case Command::NONE:
      {
        BOOST_LOG_TRIVIAL(warning) << "No command found. Skipping";
        break;
      }
      default:
        BOOST_LOG_TRIVIAL(warning) << "Command variable was not initialized. Skipping";
        break;
    }
  }
}

namespace commands
{
  void processMessage(std::shared_ptr< NewMessage > newMessage)
  {
    using namespace config;
    if (!hasMessageNecessaryFields(newMessage->getMessage()))
    {
      BOOST_LOG_TRIVIAL(info) << "Message does not contain necessary fields. Skipping";
      return;
    }
    if (newMessage->getMessage().getFromId() == config::ConfigHolder::getGroupId())
    {
      BOOST_LOG_TRIVIAL(info) << "Message from bot. Skipping";
      return;
    }
    logMessage(newMessage->getMessage());
    try
    {
      size_t pos;
      Tag tag = findTag(newMessage->getMessage().getText(), pos);
      if (tag != Tag::NONE)
      {
        BOOST_LOG_TRIVIAL(info) << "Message with tag";
        processMessageWithTag(newMessage->getMessage(), tag, pos);
        BOOST_LOG_TRIVIAL(info) << "Message tag processing finished successfully";
        return;
      }
      Command cmd = findCommand(newMessage->getMessage().getText(), pos);
      if (cmd != Command::NONE)
      {
        BOOST_LOG_TRIVIAL(info) << "Message with command";
        processMessageWithCommand(newMessage->getMessage(), cmd, pos);
        BOOST_LOG_TRIVIAL(info) << "Message command processing finished successfully";
        return;
      }
      BOOST_LOG_TRIVIAL(info) << "Neither a command nor a tag were found. Skipping";
    }
    catch (const Json::Exception &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "Json exception");
    }
    catch (const std::logic_error &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "std::logic_error");
    }
    catch (const vk::exceptions::RequestException &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "RequestException");
    }
    catch (const std::exception &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "std::excepiton");
    }
  }
}
