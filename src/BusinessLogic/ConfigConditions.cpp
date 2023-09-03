#include "ConfigConditions.hpp"
#include "Logging/Logger.hpp"

bool config::ConfigConditions::isChatSource(const config::Config &config, types::VkChatId_t vkChatId)
{
  return config.sourceChat && config.sourceChat->vkChatId == vkChatId;
}

bool config::ConfigConditions::isModeWork(const config::Config &config)
{
  return config.mode == types::Mode::WORK;
}

bool config::ConfigConditions::isModeConfig(const config::Config &config)
{
  return config.mode == types::Mode::CONFIG;
}

bool config::ConfigConditions::isAnyTargetChatPresent(const config::Config &config)
{
  return !config.targetChats.empty();
}

bool config::ConfigConditions::isVkChatIdInTargets(const config::Config &config, types::VkChatId_t vkChatId)
{
  return config.targetChats.containsVkChatId(vkChatId);
}

bool config::ConfigConditions::isChatIdInTargets(const config::Config &config, types::ChatId_t chatId)
{
  return config.targetChats.containsChatId(chatId);
}

bool config::ConfigConditions::isSourceChatSet(const config::Config &config)
{
  return config.sourceChat.has_value();
}

bool config::ConfigConditions::isUserGodlike(const config::Config &config, types::VkUserId_t vkUserId)
{
  return std::find(config.godlikeIds.cbegin(), config.godlikeIds.cend(), vkUserId) != config.godlikeIds.cend();
}

bool config::ConfigConditions::canChangeMode(const config::Config &config)
{
  if (config.mode == types::Mode::CONFIG)
    return isModeValid(config, types::Mode::WORK);
  else if (config.mode == types::Mode::WORK)
    return isModeValid(config, types::Mode::CONFIG);
  else
  {
    Logger::log(Logger::FATAL, "Config mode not work nor config. Returning false in canChangeMode()");
    return false;
  }
}

bool config::ConfigConditions::isModeValid(const config::Config &config, types::Mode mode)
{
  if (mode == types::Mode::CONFIG)
    return true;
  else if (mode == types::Mode::WORK)
    return config.sourceChat.has_value() && !config.targetChats.empty();
  else
  {
    Logger::log(Logger::FATAL, "Config mode not work nor config. Returning false in isModeValid()");
    return false;
  }
}
