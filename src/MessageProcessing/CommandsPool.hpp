#ifndef COMMANDS_POOL_HPP
#define COMMANDS_POOL_HPP

#include <vector>
#include <memory>
#include "Interfaces/CommandI.hpp"

namespace msg_proc::commands
{
  class CommandsPool
  {
  public:
    static const std::vector< std::shared_ptr< CommandI > > commands;
  };
}
#endif
