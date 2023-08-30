#include "EventProcessingStrategy.hpp"
#include "../VkApi/Callback/Event/EventObjects/NewMessage.hpp"
#include "../Logging/Logger.hpp"

void EventProcessingStrategy::processNewMessageEvent(Event_t event)
{
  using namespace vk::callback::event::objects;
  auto eventObject = std::dynamic_pointer_cast<NewMessage>(event.getEventObject());
  if (!eventObject)
  {
    Logger::log(Logger::ERROR, "EventProcessingStrategy - Precondition failed. Can not cast event object to NewMessage. Skipping event");
    return;
  }
}