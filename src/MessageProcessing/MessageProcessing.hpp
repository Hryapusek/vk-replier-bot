#ifndef MESSAGE_PROCESSING_HPP
#define MESSAGE_PROCESSING_HPP

#include <map>
#include <vector>
#include <string>
#include "../VkApi/Callback/Event/EventObjects/NewMessage.hpp"

namespace message_processing
{
  enum class Tag
  {
    ALL,
    ALL_IMPORTANT,
    NONE
  };

  const std::map< Tag, const std::vector< std::string > > tagStrings = {
    { Tag::ALL,           { "_всем_" } },
    { Tag::ALL_IMPORTANT, { "_всем_важно" } }
  };

  enum class Command
  {
    REG_TARGET,
    REG_SOURCE,
    REG_CHECKER,
    DEL_TARGET,
    DEL_TARGET_BY_NUM,
    DEL_SOURCE,
    DEL_CHECKER,
    PRINT_MAP,
    NONE
  };

  const std::map< Command, const std::vector< std::string > > commandStrings = {
    { Command::REG_TARGET,          { "/reg_target" } },
    { Command::REG_SOURCE,          { "/reg_source" } },
    { Command::REG_CHECKER,         { "/reg_checker" } },
    { Command::DEL_TARGET,          { "/del_target" } },
    { Command::DEL_TARGET_BY_NUM,   { "/del_target_by_num" } },
    { Command::DEL_SOURCE,          { "/del_source" } },
    { Command::DEL_CHECKER,         { "/del_checker" } },
    { Command::PRINT_MAP,           { "/print_map" } },
  };

  void processMessage(std::shared_ptr< vk::callback::event::objects::NewMessage >);
}

#endif
