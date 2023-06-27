#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <jsoncpp/json/json.h>
#include <string>
#include "Event/EventObjects/NewMessage.hpp"
#include "Event/Event.hpp"

namespace json_utils
{
  /// @throws Json::Exception.
  Json::Value stringToJsonValue(std::string_view s);
  NewMessage JsonValueToNewMessage(std::reference_wrapper< const Json::Value > root);
  /**
   * @throws Json::Exception.
   * @see <a href="https://dev.vk.com/api/community-events/json-schema">VK event json</a>
   * @note If event type is unsupported one - EventObject will contain nullptr.
   */
  Event parseEvent(std::reference_wrapper< const Json::Value > root);
  /**
   * @return EventType or UNKNOWN if json does not
   * contain field "type" or its value is not supported.
   */
  EventType parseEventType(std::reference_wrapper< const Json::Value > root);
}

#endif
