#include <jsoncpp/json/json.h>
#include <string>
#include "Event/EventObjects/NewMessage.hpp"

Json::Value stringToJsonValue(const std::string &str);
NewMessage JsonValueToNewMessage(const Json::Value &root);
