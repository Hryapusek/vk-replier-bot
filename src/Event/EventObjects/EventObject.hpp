#ifndef EVENTOBJECT_HPP
#define EVENTOBJECT_HPP

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

#endif
