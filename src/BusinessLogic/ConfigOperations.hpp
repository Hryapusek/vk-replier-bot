#ifndef CONFIG_OPERATIONS_HPP
#define CONFIG_OPERATIONS_HPP

#include "ConfigHolder.hpp"
#include "ConfigTypes/Chats.hpp"

namespace config
{
  class ConfigOperations
  { 
  public:
    static void addTargetChat(const Config&, types::TargetChat_t);
    static std::string getTargetsString(const Config&);
  };
}

#endif
