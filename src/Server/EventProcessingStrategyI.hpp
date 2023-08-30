#ifndef EVENT_PROCESSING_STRATEGY_HPP
#define EVENT_PROCESSING_STRATEGY_HPP

#include "../VkApi/Callback/Event/Event.hpp"

class EventProcessingStrategyI
{
protected:
  using Event_t = vk::callback::event::Event;
  
public:
  virtual void processNewMessageEvent(Event_t event) {};
};

#endif
