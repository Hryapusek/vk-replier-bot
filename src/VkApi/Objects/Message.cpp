#include "Message.hpp"

namespace vk::objects
{
  bool Message::fromDirect() const
  {
    return peer_id > 0 && peer_id < CHAT_START_ID;
  }

  bool Message::fromChat() const
  {
    return peer_id > CHAT_START_ID;
  }

  bool Message::fromGroup() const
  {
    return peer_id < 0;
  }

  Message Message::fromJson(std::reference_wrapper< Json::Value > root)
  {
    Message msg;
    msg.peer_id = root.get()["peer_id"].asInt();
    msg.text = root.get()["text"].asString();
    msg.id = root.get()["id"].asInt();
    msg.from_id = root.get()["from_id"].asInt();
    return msg;
  }
}
