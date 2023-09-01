#ifndef CONFIG_CONDITIONS_HPP
#define CONFIG_CONDITIONS_HPP

#include "ConfigTypes/Chats.hpp"
#include "ConfigHolder.hpp"

namespace conditions
{
  bool isChatSource(const config::Config &, types::ChatId_t);
  bool isModeWork(const config::Config &);
  bool isModeConfig(const config::Config &);
  bool isAnyTargetChatPresent(const config::Config &);
  bool isVkChatIdInTargets(const config::Config &, types::VkChatId_t);
  bool isChatIdInTargets(const config::Config &, types::ChatId_t);
  bool isSourceChatSet(const config::Config &);
  bool isUserGodlike(const config::Config &, types::VkUserId_t);
  bool canChangeMode(const config::Config &);
}
#endif
