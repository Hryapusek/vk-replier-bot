#include "Utils.hpp"

namespace vk::callback::event::details
{
  EventType parseEventType(const Json::Value &root)
  {
    if (!root.isMember("type") || !root["type"].isString())
      throw Json::Exception("");
    auto type = root["type"].asString();
    if (type == "message_new")
      return EventType::MESSAGE_NEW;
    else if (type == "confirmation")
      return EventType::CONFIRMATION;
    return EventType::UNKNOWN;
  }
}
