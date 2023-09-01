#ifndef COMMAND_STATUS_HPP
#define COMMAND_STATUS_HPP

#include "../Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class StatusCommand : public CommandI
  {
    using CommandI::CommandI;
  public:
    virtual void execute(const Message_t &msg);
  };
}
#endif
