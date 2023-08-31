#ifndef CONFIG_CONDITIONS_HPP
#define CONFIG_CONDITIONS_HPP

#include "../VkApi/GeneralTypes.hpp"
#include "ConfigReader.hpp"

namespace conditions
{
  bool isChatSource(const config::Config &, vk::ChatId_t);
  bool isModeWork(const config::Config &);
  bool isAnyTargetChatPresent(const config::Config &);
}
#endif
