#ifndef MESSAGE_PROCESSING_HPP
#define MESSAGE_PROCESSING_HPP

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "../Event/EventObjects/NewMessage.hpp"

namespace commands
{
  enum Tag
  {
    ALL_BLOCKS,
    ALL_BLOCKS_IMPORTANT,
    NONE
  };

  std::map<Tag, std::vector<std::string>> tagStrings = {
    {ALL_BLOCKS,           {"_всем_блокам"}},
    {ALL_BLOCKS_IMPORTANT, {"_всем_блокам_важно"}}
  };

  enum Command
  {
    REG_TARGET,
    REG_SOURCE,
    REG_CHECKER,
    DEL_TARGET_BY_ID,
    DEL_TARGET_BY_BLOCK,
    DEL_SOURCE,
    DEL_THIS_SOURCE,
    DEL_CHECKER,
    PRINT_MAP,
    NONE
  };

  std::map<Command, std::vector<std::string>> commandStrings = {
    {REG_TARGET,          {"/reg_target"}},
    {REG_SOURCE,          {"/reg_source"}},
    {REG_CHECKER,         {"/reg_checker"}},
    {DEL_TARGET_BY_ID,    {"/del_target_by_id"}},
    {DEL_TARGET_BY_BLOCK, {"/del_target_by_block"}},
    {DEL_SOURCE,          {"/del_source"}},
    {DEL_THIS_SOURCE,     {"/del_this_source"}},
    {DEL_CHECKER,         {"/del_checker"}},
    {PRINT_MAP,           {"/print_map"}},
  };

  void processMessage(std::shared_ptr< NewMessage >);
}

#endif
