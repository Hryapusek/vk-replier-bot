#include "MessagesGetByConversationMessageIdResponse.hpp"

#include "../../../JsonUtils.hpp"

namespace vk::responses::messages
{
  const std::vector<Message> &MessagesGetByConversationMessageIdResponse::getMessages()
  {
    return this->messages;
  }

  MessagesGetByConversationMessageIdResponse MessagesGetByConversationMessageIdResponse::fromJson(const Json::Value &root)
  {
    std::tuple fields = {
      JsonFieldVT("items", Json::arrayValue),
      JsonFieldT<int>("count")
    };
    MessagesGetByConversationMessageIdResponse resp;
    for (const auto &item : root["items"])
      resp.messages.push_back(Message::fromJson(item));
    return resp;
  }
}
