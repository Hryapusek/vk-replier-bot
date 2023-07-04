#include "NewMessage.hpp"
#include "../../../../JsonUtils.hpp"

namespace vk::callback::event::objects
{
  NewMessage NewMessage::fromJson(const Json::Value &root)
  {
    NewMessage newMsg;
    checkJsonField(root, JsonFieldVT("message", Json::objectValue));
    newMsg.message = Message::fromJson(root["message"]);
    return newMsg;
  }
}
