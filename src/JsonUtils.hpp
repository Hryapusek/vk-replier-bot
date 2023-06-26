#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <jsoncpp/json/json.h>
#include <string>
#include "Event/EventObjects/NewMessage.hpp"
#include "Event/Event.hpp"

namespace json_utils
{
  class InvalidJsonError: public std::exception
  {
  public:
    InvalidJsonError(const char *message) :
      message(message)
    { }
    virtual const char *what() const noexcept
    {
      return message;
    }
  private:
    const char *message;
  };
  InvalidJsonError json;
  Json::Value stringToJsonValue(std::string_view s);
  NewMessage JsonValueToNewMessage(const Json::Value &root);
  /**
   * @brief Tries to convert json to Event.
   * @throws json_utils::InvalidJsonError if can not find necessary fields.
   * @see <a href="https://dev.vk.com/api/community-events/json-schema">VK event json</a>
   * @note If event type is unsupported one - EventObject will contain nullptr.
   * @param root json that have to be
   */
  Event parseEvent(const Json::Value &root);
  /**
   * @brief Reads "type" field in json and returns @ref EventType.
   *
   * @param root json contains "type" field.
   * @return EventType or UNKNOWN if json does not
   * contain field "type" or its value is not supported.
   */
  EventType parseEventType(const Json::Value &root);
}

#endif
