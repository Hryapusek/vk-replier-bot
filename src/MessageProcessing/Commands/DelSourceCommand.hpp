#ifndef COMMAND_DEL_SOURCE_HPP
#define COMMAND_DEL_SOURCE_HPP

#include "MessageProcessing/Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class DelSourceCommand : public CommandI
  {
    using CommandI::CommandI;
  public:
    virtual void execute(const Message_t &msg);
  };
}

#endif
