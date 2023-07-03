#include "Utils.hpp"

namespace vk::callback::event::details
{
  EventType parseEventType(std::reference_wrapper< const Json::Value > root)
  {
    if (!root.get().isMember("type"))
      return EventType::UNKNOWN;
    auto type = root.get()["type"].asString();
    if (type == "message_new")
      return EventType::MESSAGE_NEW;
    else if (type == "confirmation")
      return EventType::CONFIRMATION;
    return;
  }
}
