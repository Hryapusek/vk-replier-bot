#include "ConfigOperations.hpp"
#include "Logging/Logger.hpp"

void config::ConfigOperations::addTargetChat(Config &config, types::TargetChat_t chatToAdd)
{
  config.targetChats.insert(chatToAdd);
}

std::string config::ConfigOperations::getTargetsString(const Config &config)
{
  return config.targetChats.getTargetIdsString();
}

void config::ConfigOperations::addSourceChat(Config &config, types::SourceChat_t chatToAdd)
{
  config.sourceChat = chatToAdd;
}

void config::ConfigOperations::addGodlike(Config &config, types::VkUserId_t vkUserId)
{
  config.godlikeIds.push_back(vkUserId);
}

void config::ConfigOperations::removeTargetChatById(Config &config, types::ChatId_t chatId)
{
  config.targetChats.removeByChatId(chatId);
}

void config::ConfigOperations::removeTargetChatByVkChatId(Config &config, types::VkChatId_t vkChatId)
{
  config.targetChats.removeByVkChatId(vkChatId);
}

void config::ConfigOperations::removeSourceChat(Config &config)
{
  config.sourceChat = std::nullopt;
}

void config::ConfigOperations::changeMode(Config &config)
{
  if (config.mode == types::Mode::CONFIG)
    config.mode = types::Mode::WORK;
  else if (config.mode == types::Mode::WORK)
    config.mode = types::Mode::CONFIG;
  else
  {
    Logger::log(Logger::FATAL, "Config mode not work nor config. Mode changed to config in changeMode()");
    config.mode = types::Mode::CONFIG;
  }
}

std::string config::ConfigOperations::getModeName(Config &config)
{
  static const std::map< types::Mode, std::string > modeToString
  {
    { types::Mode::WORK, "WORK" },
    { types::Mode::CONFIG, "CONFIG" },
  };
  return modeToString.at(config.mode);
}
