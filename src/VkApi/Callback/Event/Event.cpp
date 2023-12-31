#include "Event.hpp"

#include "EventObjects/NewMessage.hpp"
#include "Utils.hpp"
#include "GeneralUtils/JsonUtils.hpp"

namespace vk::callback::event
{
  Event::Event() :
    type(EventType::UNKNOWN),
    object(nullptr),
    group_id(0)
  { }

  Event::Event(EventType type, std::shared_ptr< EventObject > object, int group_id) :
    type(type),
    object(object),
    group_id(group_id)
  {  }

  EventType Event::getType()
  {
    return type;
  }

  void Event::setType(EventType type)
  {
    this->type = type;
  }

  std::shared_ptr< objects::EventObject > Event::getEventObject()
  {
    return object;
  }

  void Event::setEventObject(std::shared_ptr< EventObject > object)
  {
    this->object = object;
  }

  int Event::getGroupId()
  {
    return group_id;
  }

  void Event::setGroupId(int group_id)
  {
    this->group_id = group_id;
  }

  Event Event::fromJson(const Json::Value &root)
  {
    static const auto fields = std::tuple{
      JsonFieldT<std::string>("type"),
      JsonFieldVT("object", Json::ValueType::objectValue, false),
      JsonFieldT<int>("group_id")
    };
    checkJsonFields(root, fields);
    Event event;
    event.type = details::parseEventType(root);
    event.group_id = root["group_id"].asInt();
    switch (event.type)
    {
    case EventType::MESSAGE_NEW:
      event.object = std::make_shared< objects::NewMessage >(objects::NewMessage::fromJson(root["object"]));
      break;

    default:
      event.object = nullptr;
      break;
    }
    return event;
  }
}
