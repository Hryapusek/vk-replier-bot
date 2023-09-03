#include "EventProcessingStrategy.hpp"
#include "VkApi/Callback/Event/EventObjects/NewMessage.hpp"
#include "Logging/Logger.hpp"
#include "CommandsPool.hpp"
#include "TagsPool.hpp"

namespace msg_proc
{
  void EventProcessingStrategy::processNewMessageEvent(Event_t event)
  {
    using namespace vk::callback::event::objects;
    auto eventObject = std::dynamic_pointer_cast< NewMessage >(event.getEventObject());
    if (!eventObject)
    {
      Logger::log(Logger::ERROR, "EventProcessingStrategy - Precondition failed. Can not cast event object to NewMessage. Skipping event");
      return;
    }
    for (const auto &cmdPtr : commands::CommandsPool::commands)
    {
      if (cmdPtr->isThisTrigger(eventObject->getMessage()))
        cmdPtr->execute(eventObject->getMessage());
    }
    for (const auto &tagPtr : commands::CommandsPool::commands)
    {
      if (tagPtr->isThisTrigger(eventObject->getMessage()))
        tagPtr->execute(eventObject->getMessage());
    }
  }
  
  std::shared_ptr<EventProcessingStrategy> EventProcessingStrategy::make_strategy()
  {
    return std::shared_ptr<EventProcessingStrategy>();
  }
}
