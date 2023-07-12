#ifndef MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID_RESPONSE_HPP
#define MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID_RESPONSE_HPP

#include <vector>
#include "../../Objects/Message.hpp"

namespace vk::responses::messages
{
  using namespace objects;
  class MessagesGetByConversationMessageIdResponse
  { 
    public:
      const std::vector<Message> &getMessages();
      /// @throw Json::Exception
      static MessagesGetByConversationMessageIdResponse fromJson(const Json::Value &);
    private:
      std::vector<Message> messages;
  };
}
#endif
