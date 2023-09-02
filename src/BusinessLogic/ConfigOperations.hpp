#ifndef CONFIG_OPERATIONS_HPP
#define CONFIG_OPERATIONS_HPP

#include "ConfigHolder.hpp"
#include "ConfigTypes/SimpleTypes.hpp"

namespace config
{
  class ConfigOperations
  { 
  public:
    static void addTargetChat(Config&, types::TargetChat_t);
    static std::string getTargetsString(const Config&);
    static void addSourceChat(Config&, types::SourceChat_t);
    static void addGodlike(Config&, types::VkUserId_t);
    static void removeTargetChatById(Config&, types::ChatId_t);
    static void removeTargetChatByVkChatId(Config&, types::VkChatId_t);
    static void removeSourceChat(Config&);
    static void changeMode(Config&);
  };
}

#endif
