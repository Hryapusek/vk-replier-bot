#ifndef EVENT_HPP
#define EVENT_HPP

#include <memory>
#include "EventTypes.hpp"
#include "EventObjects/EventObject.hpp"

class Event
{
public:
  Event();
  Event(EventType type, std::shared_ptr< EventObject > object, int group_id);
  EventType getType();
  void setType(EventType type);
  std::shared_ptr< EventObject > getEventObject();
  void setEventObject(std::shared_ptr< EventObject > object);
  int getGroupId();
  void setGroupId(int group_id);

private:
  EventType type;
  std::shared_ptr< EventObject > object;
  int group_id;
};

#endif
