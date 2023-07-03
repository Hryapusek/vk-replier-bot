#ifndef MESSAGES_SEND_REQUEST_HPP
#define MESSAGES_SEND_REQUEST_HPP

#include "../BaseRequest.hpp"

namespace vk::requests::messages
{
  class MessagesSendRequest : protected details::BaseRequest
  { 
    template < class T > using vec_cref = const std::vector<T> &;
    public:
      MessagesSendRequest();
      MessagesSendRequest &user_id(int userId);
      MessagesSendRequest &random_id(int randomId);
      MessagesSendRequest &peer_id(int peerId);
      MessagesSendRequest &peer_ids(vec_cref<int> peerIds);
      MessagesSendRequest &peer_ids(str_cref peerIds);
      MessagesSendRequest &chat_id(int chatId);
      MessagesSendRequest &user_ids(vec_cref<int> userIds);
      MessagesSendRequest &message(str_cref message);
      MessagesSendRequest &forward_messages(vec_cref<int> forward_messages);
      MessagesSendRequest &forward_messages(str_cref forward_messages);
      /// @throw Json::Exception
      /// @throw RequestException
      void execute();
      
  };
}
#endif
