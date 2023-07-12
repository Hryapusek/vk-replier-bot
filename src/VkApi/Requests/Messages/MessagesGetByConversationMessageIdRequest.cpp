#include "MessagesGetByConversationMessageIdRequest.hpp"
#include "../Utils.hpp"

namespace vk::requests::messages
{
  using Request = MessagesGetByConversationMessageIdRequest;
  using Response = ::vk::responses::messages::MessagesGetByConversationMessageIdResponse;

  Request::MessagesGetByConversationMessageIdRequest() :
    BaseRequest("messages.send")
  {  }

  Request & Request::peer_id(int peerId)
  {
    params.Add({{"peer_id", std::to_string(peerId)}});
    return *this;
  }

  Request & Request::conversation_message_ids(vec_cref<int> peerIds)
  {
    params.Add({{"peer_ids", details::vectorToString(peerIds)}});
    return *this;
  }

  Request & Request::conversation_message_ids(str_cref peerIds)
  {
    params.Add({{"peer_ids", peerIds}});
    return *this;
  }

  Response Request::execute()
  {
    send();
    return Response::fromJson(responseJson);
  }
}
