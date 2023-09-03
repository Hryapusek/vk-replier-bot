#include "MsgUtils.hpp"
#include <regex>
#include "Logging/Logger.hpp"
#include "VkApi/Requests/Messages/MessagesSendRequest.hpp"

namespace msg_proc
{
  MsgUtils::CIterator_t MsgUtils::findWord(str_cref text, str_cref wordToFind)
  {
    std::string regexText = "(^|[ \n]+)" + wordToFind + "($|[ \n]+)";
    std::regex regex(regexText);
    std::smatch res;
    bool isFound = std::regex_search(text, res, regex);
    if (isFound)
      return res[0].first;
    return text.cend();
  }

  void MsgUtils::sendSimpleMessage(vk::types::PeerId_t destPeerId, str_cref text)
  {
    using namespace vk::requests::messages;
    MessagesSendRequest().random_id(0).peer_id(destPeerId).message(text).execute();
  }

  void MsgUtils::sendMessageToAllTargets(str_cref text, int fwd_conv_msg_id, str_cref targetChatsStrings, vk::types::ChatId_t sourceChatVkId)
  {
    using namespace vk::requests::messages;
    MessagesSendRequest req;
    {
      ForwardParam fwdParam;
      fwdParam.conversation_message_ids = { fwd_conv_msg_id };
      fwdParam.peer_id = sourceChatVkId;
      req
      .peer_ids(targetChatsStrings)
      .random_id(0)
      .forward(fwdParam);
    }
    if (!text.empty())
      req.message(text);
    req.execute();
  }

  void MsgUtils::sendResponseMessage(vk::types::PeerId_t destPeerId, str_cref text)
  {
    sendSimpleMessage(destPeerId, text);
  }

  void MsgUtils::logAndSendResponseMessage(vk::types::PeerId_t destPeerId, str_cref text)
  {
    Logger::log(Logger::INFO, text);
    sendSimpleMessage(destPeerId, text);
  }

  void MsgUtils::sendErrorResponseMessage(vk::types::PeerId_t destPeerId, str_cref errorText)
  {
    sendSimpleMessage(destPeerId, errorText);
  }

  void MsgUtils::logAndSendErrorResponseMessage(vk::types::PeerId_t destPeerId, str_cref errorText)
  {
    Logger::log(Logger::INFO, errorText);
    sendSimpleMessage(destPeerId, errorText);
  }

  void MsgUtils::logTextTriggerName(str_cref textTriggerName)
  {
    Logger::log(Logger::INFO, "In TextTrigger execution with name ", textTriggerName);
  }
}