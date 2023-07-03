#ifndef VKAPI_EVENT_UTILS_HPP
#define VKAPI_EVENT_UTILS_HPP

#include <jsoncpp/json/json.h>
#include "EventTypes.hpp"

namespace vk::callback::event::details
{
 /**
 * @return EventType or UNKNOWN if json does not
 * contain field "type" or its value is not supported.
 */
  EventType parseEventType(std::reference_wrapper< const Json::Value > root);
}

#endif
