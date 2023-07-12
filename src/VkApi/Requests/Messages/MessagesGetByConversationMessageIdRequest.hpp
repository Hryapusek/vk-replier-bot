#ifndef MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID_REQUEST_HPP
#define MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID_REQUEST_HPP

#include "../BaseRequest.hpp"
#include "../../Responses/Messages/MessagesGetByConversationMessageIdResponse.hpp"

namespace vk::requests::messages
{
  class MessagesGetByConversationMessageIdRequest : protected details::BaseRequest
  { 
    template < class T > using vec_cref = const std::vector<T> &;
    using Request = MessagesGetByConversationMessageIdRequest;
    public:
      MessagesGetByConversationMessageIdRequest();
      Request &peer_id(int peerId);
      Request &conversation_message_ids(vec_cref<int> peerIds);
      Request &conversation_message_ids(str_cref peerIds);
      /// @throw Json::Exception
      /// @throw RequestException
      ::vk::responses::messages::MessagesGetByConversationMessageIdResponse execute();
      virtual ~MessagesGetByConversationMessageIdRequest() = default;
  };
}
#endif
