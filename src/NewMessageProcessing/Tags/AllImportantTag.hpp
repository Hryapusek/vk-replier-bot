#ifndef TAG_ALL_IMPORTANT_HPP
#define TAG_ALL_IMPORTANT_HPP

#include "../../VkApi/Objects/Message.hpp"
#include "../Interfaces/TagI.hpp"

namespace msg_proc::tags
{
  class AllImportantTag : public TagI
  {
    using TagI::TagI;
  public:
    virtual void execute(const Message_t &msg);
  };
}

#endif
