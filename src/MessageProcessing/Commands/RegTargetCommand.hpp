#ifndef COMMAND_REG_TARGET_HPP
#define COMMAND_REG_TARGET_HPP

#include "MessageProcessing/Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class RegTargetCommand : public CommandI
  {
    using CommandI::CommandI;
  public:
    virtual void execute(const Message_t &msg);
  };
}
#endif
