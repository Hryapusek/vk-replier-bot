#include "Message.hpp"
#include "../../JsonUtils.hpp"

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
    const bool necessary = true;
    const static auto fields = std::tuple{
      JsonFieldT<int>("id",           !necessary),
      JsonFieldT<int>("peer_id",      !necessary),
      JsonFieldT<int>("from_id",      !necessary),
      JsonFieldT<std::string>("text", !necessary),
    };
    checkJsonFields(root.get(), std::move(fields));
    Message msg;
    msg.id = root.get()["id"].asInt();
    msg.peer_id = root.get()["peer_id"].asInt();
    msg.from_id = root.get()["from_id"].asInt();
    msg.text = root.get()["text"].asString();
    return msg;
  }
}
