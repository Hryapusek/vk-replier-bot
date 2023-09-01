#ifndef COMMAND_REG_GODLIKE_HPP
#define COMMAND_REG_GODLIKE_HPP

#include "../Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class RegGodlikeCommand : public CommandI
  {
    using CommandI::CommandI;
  public:
    virtual void execute(const Message_t &msg);
  };
}
#endif
