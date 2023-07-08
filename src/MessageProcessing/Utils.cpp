#include "Utils.hpp"
#include <boost/log/trivial.hpp>
#include "../ConfigReader.hpp"

namespace message_processing::utils
{
  using namespace vk::objects;
  using namespace vk::requests::messages;
  using namespace config;

  bool skipWord(std::string::const_iterator &beg, str_cref text)
  {
    while (beg != text.cend() && !isspace(*beg))
      ++beg;
    while (beg != text.cend() && (*beg == ' ' || *beg == '\t'))
      ++beg;
    return !(beg == text.cend() || *beg == '\n');
  }

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

  std::string extractTitle(str_cref text, size_t pos)
  {
    std::string title;
    auto beg = std::string::const_iterator(&text[pos]);
    if (!skipWord(beg, text) || *beg != '"')
      return title;
    else
      return extractString(beg, text);
  }

  void logAndSendErrorMessage(MessagesSendRequest &req, str_cref commandName, str_cref errorMessage)
  {
    BOOST_LOG_TRIVIAL(warning) << commandName << ": " << errorMessage << ". Skipping message";
    req.message(errorMessage).execute();
  }

  void sendMessageToAllTargets(str_cref text, int fwd_msg_id)
  {
    MessagesSendRequest()
    .peer_ids(ConfigHolder::getTargetIds())
    .random_id(0)
    .forward_messages(std::to_string(fwd_msg_id))
    .message(text)
    .execute();
  }

  bool checkIfCommandFromChat(const Message &message, str_cref commandName, str_cref errorMessage)
  {
    return checkIf(message.fromChat(), commandName, errorMessage);
  }

  bool checkIfChatIsNotSource(int peerId, MessagesSendRequest &req, str_cref commandName, str_cref errorMessage)
  {
    auto sourceChat = ConfigHolder::getReadOnlyConfig().config.sourceChat;
    bool cond = sourceChat && sourceChat->peer_id == peerId;
    return checkIf(cond, req, commandName, errorMessage);
  }

  bool checkMode(Mode mode, str_cref commandName, str_cref errorMessage)
  {
    if (ConfigHolder::getMode() != mode)
    {
      std::string postfix;
      switch (mode)
      {
        case Mode::CONFIG:
          postfix = " - mode is not CONFIG";
          break;

        case Mode::WORK:
          postfix = " - mode is not WORK";
          break;
        
        default:
          postfix = "- CAN NOT CHECK THE MODE. IT'S NOT INITIALIZED!!!";
      }
      BOOST_LOG_TRIVIAL(warning) << commandName << ": " << errorMessage << postfix;
      return false;
    }
    return true;
  }

  bool checkIfSourceChatPresent(str_cref commandName, str_cref errorMessage)
  {    
    bool cond = config::ConfigHolder::getReadOnlyConfig().config.sourceChat.has_value();
    return checkIf(cond, commandName, errorMessage);
  }

  bool checkIfChatIsNotSource(int peerId, str_cref commandName, str_cref errorMessage)
  {
    auto sourceChat = config::ConfigHolder::getReadOnlyConfig().config.sourceChat;
    bool cond = !(sourceChat && sourceChat->peer_id == peerId);
    return checkIf(cond, commandName, errorMessage);
  }

  bool checkIfChatIsSource(int peerId, str_cref commandName, str_cref errorMessage)
  {
    auto sourceChat = config::ConfigHolder::getReadOnlyConfig().config.sourceChat;
    bool cond = sourceChat && sourceChat->peer_id == peerId;
    return checkIf(cond, commandName, errorMessage);
  }

  bool checkIfChatIsSource(int peerId, vk::requests::messages::MessagesSendRequest & req, str_cref commandName, str_cref errorMessage)
  {
    auto sourceChat = config::ConfigHolder::getReadOnlyConfig().config.sourceChat;
    bool cond = sourceChat && sourceChat->peer_id == peerId;
    return checkIf(cond, req, commandName, errorMessage);
  }

  bool checkIfPeerIdInTargetsTable(const Message &message, MessagesSendRequest &req, str_cref commandName, str_cref errorMessage)
  {
    bool cond = config::ConfigHolder::getReadOnlyConfig().config.targetsTable.containsPeerId(message.getPeerId());
    return checkIf(cond, req, commandName, errorMessage);
  }

  bool checkIfPeerIdNotInTargetsTable(const Message &message, MessagesSendRequest &req, str_cref commandName, str_cref errorMessage)
  {
    bool cond = !config::ConfigHolder::getReadOnlyConfig().config.targetsTable.containsPeerId(message.getPeerId());
    return checkIf(cond, req, commandName, errorMessage);
  }

  bool checkIfFromDirect(const Message & message, str_cref commandName, str_cref errorMessage)
  {
    return checkIf(message.fromDirect(), commandName, errorMessage);
  }

  bool checkIfChecker(const Message &message, MessagesSendRequest &req, str_cref commandName, str_cref errorMessage)
  {
    const auto &checkers = config::ConfigHolder::getReadOnlyConfig().config.statusCheckersIds;
    bool cond = std::find(checkers.cbegin(), checkers.cend(), message.getPeerId()) != checkers.end();
    return checkIf(cond, req, commandName, errorMessage);
  }

  bool checkIfNotChecker(const Message &message, str_cref commandName, str_cref errorMessage)
  {
    const auto &checkers = config::ConfigHolder::getReadOnlyConfig().config.statusCheckersIds;
    bool cond = std::find(checkers.cbegin(), checkers.cend(), message.getPeerId()) == checkers.end();
    return checkIf(cond, commandName, errorMessage);
  }

  bool checkIf(bool cond, MessagesSendRequest & req, str_cref commandName, str_cref errorMessage)
  {
    if (!cond)
      logAndSendErrorMessage(req, commandName, errorMessage);
    return cond;
  }

  bool checkIf(bool cond, str_cref commandName, str_cref errorMessage)
  {
    if (!cond)
      BOOST_LOG_TRIVIAL(warning) << commandName << ": " << errorMessage;
    return cond;
  }
}
