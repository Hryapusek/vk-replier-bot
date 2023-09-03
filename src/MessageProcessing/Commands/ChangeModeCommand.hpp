#ifndef COMMAND_CHANGE_MODE_HPP
#define COMMAND_CHANGE_MODE_HPP

#include "MessageProcessing/Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class ChangeModeCommand : public CommandI
  {
    using CommandI::CommandI;
  public:
    virtual void execute(const Message_t &msg);
  };
}

#endif
