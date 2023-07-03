#ifndef VKAPI_EVENTOBJECT_HPP
#define VKAPI_EVENTOBJECT_HPP

namespace vk::callback::event::objects
{
/**
 * @brief Abstract base class for all events. You should cast it to actual event
 * depending on event type from json.
 */
  class EventObject
  {
  private:
    virtual void foo() = 0;
  public:
    virtual ~EventObject() = default;
  };
}

#endif
