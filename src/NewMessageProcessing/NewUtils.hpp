#ifndef MSG_UTILS_HPP
#define MSG_UTILS_HPP

#include <string>
#include "../VkApi/GeneralTypes.hpp"

namespace msg_proc
{
  class MsgUtils
  {
  public:
    using CIterator_t = std::string::const_iterator;
    using str_cref = const std::string &;

    static CIterator_t findWord(str_cref text, str_cref wordToFind);
    static void sendSimpleMessage(vk::types::PeerId_t destPeerId, str_cref text);
    static void sendMessageToAllTargets(str_cref text, int fwd_conv_msg_id, str_cref targetChatsString, vk::types::ChatId_t sourceChatVkId);
    static void sendResponseMessage(vk::types::PeerId_t destPeerId, str_cref text);
    static void sendErrorResponseMessage(vk::types::PeerId_t destPeerId, str_cref errorText);
  };
}

#endif
