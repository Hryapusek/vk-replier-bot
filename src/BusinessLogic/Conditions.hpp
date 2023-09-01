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
  bool isChatInTargets(const config::Config &, types::VkChatId_t);
  bool isChatIdBusy(const config::Config &, types::ChatId_t);
}
#endif
