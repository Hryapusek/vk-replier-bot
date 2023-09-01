#ifndef COMMAND_DEL_TARGET_BY_NUM_HPP
#define COMMAND_DEL_TARGET_BY_NUM_HPP

#include "../Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class DelTargetByNumCommand : public CommandI
  {
    using CommandI::CommandI;
  public:
    virtual void execute(const Message_t &msg);
  };
}
#endif
