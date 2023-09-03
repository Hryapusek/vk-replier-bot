#include "Server/EventProcessingStrategyI.hpp"

namespace msg_proc
{
  class TagsCmdsProcessingStrategy: public EventProcessingStrategyI
  {
  public:
    virtual bool processNewMessageEvent(Event_t event) override;
    static std::shared_ptr<TagsCmdsProcessingStrategy> make_strategy();
  };
}
