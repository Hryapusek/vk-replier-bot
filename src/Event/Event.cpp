#include "Event.hpp"

Event::Event() :
  type(UNKNOWN),
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

std::shared_ptr< EventObject > Event::getEventObject()
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
