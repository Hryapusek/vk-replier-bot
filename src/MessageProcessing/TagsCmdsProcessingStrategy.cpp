#include "TagsCmdsProcessingStrategy.hpp"
#include "VkApi/Callback/Event/EventObjects/NewMessage.hpp"
#include "VkApi/Objects/Message.hpp"
#include "Logging/Logger.hpp"
#include "CommandsPool.hpp"
#include "TagsPool.hpp"

namespace
{
  bool hasNecessaryFields(const vk::objects::Message &msg)
  {
    return msg.hasFromId() && msg.hasPeerId() && msg.hasConversationMessageId() && msg.hasText();
  }

  void logNewMessage(const vk::objects::Message &msg)
  {
    Logger::log(Logger::INFO, "Message: \n",
      "FromId: ", msg.getFromId(), "\n",
      "PeerId: ", msg.getPeerId(), "\n",
      "Cmvid: ", msg.getConversationMessageId(), "\n",
      "Text: ", msg.getText()
      );
  }
}

namespace msg_proc
{
  bool TagsCmdsProcessingStrategy::processNewMessageEvent(Event_t event)
  {
    using namespace vk::callback::event::objects;
    auto eventObject = std::dynamic_pointer_cast< NewMessage >(event.getEventObject());
    const auto &newMessage = eventObject->getMessage();
    if (!eventObject)
    {
      Logger::log(Logger::ERROR, "EventProcessingStrategy - Precondition failed. Can not cast event object to NewMessage. Skipping event");
      return false;
    }
    if (!hasNecessaryFields(newMessage))
    {
      Logger::log(Logger::INFO, "Message does not have necessary fields. Skipping event");
      return false;
    }
    logNewMessage(newMessage);
    // walk through commands
    for (const auto &cmdPtr : commands::CommandsPool::commands)
    {
      if (cmdPtr->isThisTrigger(newMessage))
      {
        Logger::log(Logger::INFO, "A command found. Executing it...");
        try
        {
          cmdPtr->execute(newMessage);
          Logger::log(Logger::INFO, "Execution finished successfully");
        }
        catch (const std::exception &e)
        {
          Logger::log(Logger::WARNING, "Exception while command execution: ", e.what());
        }
        return true;
      }
    }

    // walk through tags
    for (const auto &tagPtr : tags::TagsPool::tags)
    {
      if (tagPtr->isThisTrigger(newMessage))
      {
        Logger::log(Logger::INFO, "A tag found. Executing it...");
        try
        {
          tagPtr->execute(newMessage);
          Logger::log(Logger::INFO, "Execution finished successfully");
        }
        catch (const std::exception &e)
        {
          Logger::log(Logger::WARNING, "Exception while tag execution: ", e.what());
        }
        return true;
      }
    }
    return false;
  }

  [[nodiscard]] std::shared_ptr< TagsCmdsProcessingStrategy > TagsCmdsProcessingStrategy::make_strategy()
  {
    return std::make_shared< TagsCmdsProcessingStrategy >();
  }
}
