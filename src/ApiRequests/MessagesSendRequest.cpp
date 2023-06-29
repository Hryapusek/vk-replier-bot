#include "MessagesSendRequest.hpp"
#include "Utils.hpp"

namespace vk
{
  MessagesSendRequest::MessagesSendRequest() :
    BaseRequest("messages.send")
  {  }

  MessagesSendRequest &vk::MessagesSendRequest::user_id(int userId)
  {
    params.Add({{"user_id", std::to_string(userId)}});
    return *this;
  }

  MessagesSendRequest & MessagesSendRequest::random_id(int randomId)
  {
    params.Add({{"random_id", std::to_string(randomId)}});
    return *this;
  }

  MessagesSendRequest & MessagesSendRequest::peer_ids(vec_cref<int> peerIds)
  {
    params.Add({{"peer_ids", vectorToString(peerIds)}});
    return *this;  
  }

  MessagesSendRequest & MessagesSendRequest::chat_id(int chatId)
  {
    params.Add({{"chat_id", std::to_string(chatId)}});
    return *this; 
  }

  MessagesSendRequest & MessagesSendRequest::user_ids(vec_cref<int> userIds)
  {
    params.Add({{"user_ids", vectorToString(userIds)}});
    return *this; 
  }

  MessagesSendRequest & MessagesSendRequest::message(str_cref message)
  {
    params.Add({{"message", message}});
    return *this; 
  }

  MessagesSendRequest & MessagesSendRequest::forward_messages(vec_cref<int> forward_messages)
  {
    params.Add({{"forward_messages", vectorToString(forward_messages)}});
    return *this;
  }

  MessagesSendRequest & MessagesSendRequest::forward_messages(str_cref forward_messages)
  {
    params.Add({{"forward_messages", forward_messages}});
    return *this; 
  }

  void MessagesSendRequest::execute()
  {
    send();
  }
}
