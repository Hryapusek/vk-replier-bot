#include "MessagesGetByConversationMessageIdRequest.hpp"
#include "../Utils.hpp"

namespace vk::requests::messages
{
  using Request = MessagesGetByConversationMessageIdRequest;
  using Response = ::vk::responses::messages::MessagesGetByConversationMessageIdResponse;

  Request::MessagesGetByConversationMessageIdRequest() :
    BaseRequest("messages.getByConversationMessageId")
  {  }

  Request & Request::peer_id(int peerId)
  {
    params.Add({{"peer_id", std::to_string(peerId)}});
    return *this;
  }

  Request & Request::conversation_message_ids(vec_cref<int> conversation_message_ids)
  {
    params.Add({{"conversation_message_ids", details::vectorToString(conversation_message_ids)}});
    return *this;
  }

  Request & Request::conversation_message_ids(str_cref conversation_message_ids)
  {
    params.Add({{"conversation_message_ids", conversation_message_ids}});
    return *this;
  }

  Response Request::execute()
  {
    send();
    return Response::fromJson(responseJson);
  }
}
