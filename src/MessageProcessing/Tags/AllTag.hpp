#ifndef TAG_ALL_HPP
#define TAG_ALL_HPP

#include "MessageProcessing/Interfaces/TagI.hpp"

namespace msg_proc::tags
{
  class AllTag : public TagI
  {
    using TagI::TagI;
  public:
    virtual void execute(const Message_t &msg);
  };
}

#endif
