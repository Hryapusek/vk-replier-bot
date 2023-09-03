#include "Server/EventProcessingStrategyI.hpp"

namespace msg_proc
{
  class EventProcessingStrategy: public EventProcessingStrategyI
  {
  public:
    virtual void processNewMessageEvent(Event_t event) override;
    static std::shared_ptr<EventProcessingStrategy> make_strategy();
  };
}
