#ifndef COMMANDS_POOL_HPP
#define COMMANDS_POOL_HPP

#include <vector>
#include <memory>
#include "CommandI.hpp"

class CommandsPool
{
  static const std::vector<std::shared_ptr<CommandI>> vec;
};

#endif
