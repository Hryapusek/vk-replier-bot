#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <jsoncpp/json/json.h>
#include <string>
#include "Event/EventObjects/NewMessage.hpp"
#include "Event/Event.hpp"

/// @throws Json::Exception.
Json::Value stringToJson(std::string_view s);

#endif
