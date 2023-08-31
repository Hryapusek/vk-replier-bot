#ifndef TAG_ALL_HPP
#define TAG_ALL_HPP

#include "../../VkApi/Objects/Message.hpp"
#include "../TagI.hpp"

namespace tags
{
  class AllTag : public TagI
  {
  public:
    virtual void execute(const Message_t &msg);
    virtual bool isThisTextTrigger(const Message_t &msg);
  };
}
#endif
