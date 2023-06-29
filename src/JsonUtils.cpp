#include "JsonUtils.hpp"

namespace json_utils
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

  Json::Value stringToJsonValue(std::string_view s)
  {
    Json::Value root;
    Json::CharReaderBuilder builder;
    const std::unique_ptr< Json::CharReader > reader(builder.newCharReader());
    if (!reader->parse(s.data(), s.data() + s.length(), &root, nullptr))
      throw Json::Exception("");
    return root;
  }
}
