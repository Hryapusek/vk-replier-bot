#include <jsoncpp/json/json.h>
#include <string>
#include "Event/EventObjects/NewMessage.hpp"

namespace json_utils
{
  Json::Value stringToJsonValue(std::string_view s);
  NewMessage JsonValueToNewMessage(const Json::Value &root);
}
