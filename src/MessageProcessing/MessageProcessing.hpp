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
    ALL,
    ALL_IMPORTANT,
    NONE
  };

  const std::map<Tag, std::vector<std::string>> tagStrings = {
    {ALL,           {"_всем_"}},
    {ALL_IMPORTANT, {"_всем_важно"}}
  };

  enum Command
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

  const std::map<Command, std::vector<std::string>> commandStrings = {
    {REG_TARGET,          {"/reg_target"}},
    {REG_SOURCE,          {"/reg_source"}},
    {REG_CHECKER,         {"/reg_checker"}},
    {DEL_TARGET,          {"/del_target"}},
    {DEL_TARGET_BY_NUM,   {"/del_target_by_num"}},
    {DEL_SOURCE,          {"/del_source"}},
    {DEL_CHECKER,         {"/del_checker"}},
    {PRINT_MAP,           {"/print_map"}},
  };

  void processMessage(std::shared_ptr< NewMessage >);
}

#endif
