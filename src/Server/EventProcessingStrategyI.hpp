#ifndef EVENT_PROCESSING_STRATEGY_HPP
#define EVENT_PROCESSING_STRATEGY_HPP

#include "VkApi/Callback/Event/Event.hpp"

class EventProcessingStrategyI
{
public:
  using Event_t = vk::callback::event::Event;
  virtual bool processNewMessageEvent([[maybe_unused]] Event_t event) { return false; };
};

#endif
