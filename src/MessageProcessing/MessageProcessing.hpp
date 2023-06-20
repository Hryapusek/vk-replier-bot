#ifndef MESSAGE_PROCESSING_HPP
#define MESSAGE_PROCESSING_HPP

#include <memory>
#include "../Event/EventObjects/NewMessage.hpp"

namespace commands
{
  enum Tag
  {
    ALL_BLOCKS,
    ALL_BLOCKS_IMPORTANT,
    NONE
  };

  enum Command
  {
    REG_TARGET,
    REG_SOURCE,
    REG_CHECKER
  };

  void processMessage(std::shared_ptr< NewMessage >);
}

#endif
