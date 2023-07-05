#include "Message.hpp"
#include "../../JsonUtils.hpp"

namespace vk::objects
{
  const int Message::CHAT_START_ID = 2e9;

  bool Message::fromDirect() const
  {
    return *peer_id > 0 && *peer_id < Message::CHAT_START_ID;
  }

  bool Message::fromChat() const
  {
    return *peer_id > Message::CHAT_START_ID;
  }

  bool Message::fromGroup() const
  {
    return *peer_id < 0;
  }

  Message Message::fromJson(const Json::Value &root)
  {
    const bool necessary = true;
    const static auto fields = std::tuple{
      JsonFieldT< int >("id",           !necessary),
      JsonFieldT< int >("peer_id",      !necessary),
      JsonFieldT< int >("from_id",      !necessary),
      JsonFieldT< std::string >("text", !necessary),
    };
    checkJsonFields(root, fields);
    Message msg;
    if (root.isMember("id"))
      msg.id = root["id"].asInt();
    if (root.isMember("peer_id"))
      msg.peer_id = root["peer_id"].asInt();
    if (root.isMember("from_id"))
      msg.from_id = root["from_id"].asInt();
    if (root.isMember("text"))
      msg.text = root["text"].asString();
    return msg;
  }
}
