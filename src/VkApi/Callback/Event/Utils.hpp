#ifndef VKAPI_EVENT_UTILS_HPP
#define VKAPI_EVENT_UTILS_HPP

#include <jsoncpp/json/json.h>
#include "EventTypes.hpp"

namespace vk::callback::event::details
{
 /**
 * @return EventType or UNKNOWN if value of field "type"
 *  is not supported.
 * 
 * @throw Json::Exception
 */
  EventType parseEventType(const Json::Value &);
}

#endif
