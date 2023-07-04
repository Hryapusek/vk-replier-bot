#include "NewMessage.hpp"
#include "../../../../JsonUtils.hpp"

namespace vk::callback::event::objects
{
  NewMessage NewMessage::fromJson(std::reference_wrapper< Json::Value > root)
  {
    NewMessage newMsg;
    checkJsonField(root.get(), JsonFieldVT("message", Json::objectValue));
    newMsg.message = Message::fromJson(root.get()["message"]);
    return newMsg;
  }
}
