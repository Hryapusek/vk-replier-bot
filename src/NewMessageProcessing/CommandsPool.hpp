#ifndef COMMANDS_POOL_HPP
#define COMMANDS_POOL_HPP

#include <vector>
#include <memory>
#include "TextTriggerI.hpp"

class CommandsPool
{
  static const std::vector<std::shared_ptr<TextTriggerI>> vec;
};

#endif
