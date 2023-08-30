#include "../Server/EventProcessingStrategyI.hpp"

class EventProcessingStrategy : EventProcessingStrategyI
{
  virtual void processNewMessageEvent(Event_t event) override;
};
