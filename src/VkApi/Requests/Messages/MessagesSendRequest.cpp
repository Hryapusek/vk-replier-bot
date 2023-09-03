#include "MessagesSendRequest.hpp"
#include "VkApi/Requests/Utils.hpp"

namespace vk::requests::messages
{
  using namespace details;

  MessagesSendRequest::MessagesSendRequest() :
    BaseRequest("messages.send")
  {  }

  MessagesSendRequest &MessagesSendRequest::user_id(int userId)
  {
    params.Add({ { "user_id", std::to_string(userId) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::random_id(int randomId)
  {
    params.Add({ { "random_id", std::to_string(randomId) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::peer_id(int peerId)
  {
    params.Add({ { "peer_id", std::to_string(peerId) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::peer_ids(vec_cref< int > peerIds)
  {
    params.Add({ { "peer_ids", vectorToString(peerIds) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::peer_ids(str_cref peerIds)
  {
    params.Add({ { "peer_ids", peerIds } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::chat_id(int chatId)
  {
    params.Add({ { "chat_id", std::to_string(chatId) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::user_ids(vec_cref< int > userIds)
  {
    params.Add({ { "user_ids", vectorToString(userIds) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::message(str_cref message)
  {
    params.Add({ { "message", message } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::forward_messages(vec_cref< int > forward_messages)
  {
    params.Add({ { "forward_messages", vectorToString(forward_messages) } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::forward_messages(str_cref forward_messages)
  {
    params.Add({ { "forward_messages", forward_messages } });
    return *this;
  }

  MessagesSendRequest &MessagesSendRequest::forward(ForwardParam fwdStruct)
  {
    Json::Value forward(Json::objectValue);
    if (fwdStruct.peer_id)
      forward["peer_id"] = *fwdStruct.peer_id;
    if (fwdStruct.owner_id)
      forward["owner_id"] = *fwdStruct.owner_id;
    if (!fwdStruct.conversation_message_ids.empty())
      forward["conversation_message_ids"] = vectorToString(fwdStruct.conversation_message_ids);
    if (!fwdStruct.message_ids.empty())
      forward["message_ids"] = vectorToString(fwdStruct.message_ids);
    Json::StreamWriterBuilder builder;
    params.Add({ { "forward", Json::writeString(builder, forward) } });
    return *this;
  }

  void MessagesSendRequest::execute()
  {
    send();
  }
}
