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

  void logAndSendErrorMessage(MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    BOOST_LOG_TRIVIAL(warning) << command << ": " << errorMessage << ". Skipping message";
    req.message(errorMessage).execute();
  }

  bool checkIfCommandFromChat(const Message &message, str_cref command)
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
    auto sourceChat = ConfigHolder::getReadOnlyConfig().config.sourceChat;
    if (sourceChat && sourceChat.value().peer_id == message.getPeerId())
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  bool checkIfChatPresentInTable(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    if (ConfigHolder::getReadOnlyConfig().config.targetsTable.containsPeerId(message.getPeerId()))
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }

  bool checkMode(Mode mode, str_cref errorMessage)
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
      }
      BOOST_LOG_TRIVIAL(warning) << errorMessage << postfix;
      return false;
    }
    return true;
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

  void sendMessageToAllTargets(str_cref text, int fwd_msg_id)
  {
    MessagesSendRequest req;
    req.peer_ids(config::ConfigHolder::getTargetIds())
    .random_id(0)
    .message(text)
    .forward_messages(std::to_string(fwd_msg_id))
    .execute();
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
}
