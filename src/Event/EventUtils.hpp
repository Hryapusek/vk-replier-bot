#ifndef EVENT_UTILS_HPP
#define EVENT_UTILS_HPP

#include "Event.hpp"

#include <jsoncpp/json/json.h>

namespace json_utils
{
  Event parseEvent(Json::Value root);
}

#endif
