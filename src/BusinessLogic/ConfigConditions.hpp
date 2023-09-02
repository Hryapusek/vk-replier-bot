#ifndef CONFIG_CONDITIONS_HPP
#define CONFIG_CONDITIONS_HPP

#include "ConfigTypes/SimpleTypes.hpp"
#include "ConfigHolder.hpp"

namespace config
{
  class ConfigConditions
  {
  public: // TODO fix includes because JsonUtils.hpp was moved
    static bool isChatSource(const config::Config &, types::VkChatId_t);
    static bool isModeWork(const config::Config &);
    static bool isModeConfig(const config::Config &);
    static bool isAnyTargetChatPresent(const config::Config &);
    static bool isVkChatIdInTargets(const config::Config &, types::VkChatId_t);
    static bool isChatIdInTargets(const config::Config &, types::ChatId_t);
    static bool isSourceChatSet(const config::Config &);
    static bool isUserGodlike(const config::Config &, types::VkUserId_t);
    static bool canChangeMode(const config::Config &);
    static bool isModeValid(const config::Config &, types::Mode);
  };
}
#endif
