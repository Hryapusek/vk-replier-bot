#ifndef VKAPI_EVENT_HPP
#define VKAPI_EVENT_HPP

#include <memory>
#include <jsoncpp/json/json.h>
#include "EventTypes.hpp"
#include "EventObjects/EventObject.hpp"

namespace vk::callback::event
{
  class Event
  {
    using EventObject = objects::EventObject;
  public:
    Event();
    Event(EventType type, std::shared_ptr< EventObject > object, int group_id);
    EventType getType();
    void setType(EventType type);
    std::shared_ptr< EventObject > getEventObject();
    void setEventObject(std::shared_ptr< EventObject > object);
    int getGroupId();
    void setGroupId(int group_id);
    /**
     * @throw Json::Exception.
     * @see <a href="https://dev.vk.com/api/community-events/json-schema">VK event json</a>
     * @note If event type is unsupported one - EventObject will contain nullptr.
     */
    static Event fromJson(std::reference_wrapper< Json::Value >);

  private:
    EventType type;
    std::shared_ptr< EventObject > object;
    int group_id;
  };
}

#endif
