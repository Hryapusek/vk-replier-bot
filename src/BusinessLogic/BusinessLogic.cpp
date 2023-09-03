#include "BusinessLogic.hpp"
#include "Logging/Logger.hpp"
#include "VkApi/Requests/Init.hpp"
#include "ConfigConditions.hpp"
#include "ConfigTypes/SimpleTypes.hpp"
#include "ConfigOperations.hpp"

const std::string BusinessLogic::configPath = "config.json";
const std::string BusinessLogic::logFilePath = "logfile.log";

using namespace config;
using CfgConds = ConfigConditions;

namespace
{
  void initVkRequests()
  {
    using namespace vk::requests;
    if (ConfigHolder::hasBaseUrl())
      initRequests(ConfigHolder::getToken(), ConfigHolder::getV(), ConfigHolder::getBaseUrl());
    else
      initRequests(ConfigHolder::getToken(), ConfigHolder::getV());
  }
}

void BusinessLogic::init()
{
  Logger::init(logFilePath);
  ConfigHolder::init(configPath);
  initVkRequests();
}

Result< std::string > BusinessLogic::getTagAllString(VkChatId_t callerChatId)
{
  auto configWrapper = ConfigHolder::getReadOnlyConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isModeWork(config))
    return make_error_result("Mode is not work");
  if (!CfgConds::isChatSource(config, callerChatId))
    return make_error_result("Chat is not source");
  if (!CfgConds::isAnyTargetChatPresent(config))
    return make_error_result("No target chats were found");
  return ConfigOperations::getTargetsString(config);
}

Result< void > BusinessLogic::addChatToTaget(Chat_t chatToAdd)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isModeConfig(config))
    return make_error_result("Mode is not config");
  if (CfgConds::isChatSource(config, chatToAdd.vkChatId))
    return make_error_result("Chat is source");
  if (CfgConds::isVkChatIdInTargets(config, chatToAdd.vkChatId))
    return make_error_result("Chat is already target");
  if (chatToAdd.chatId && CfgConds::isChatIdInTargets(config, *chatToAdd.chatId))
    return make_error_result("Requested id is already busy");
  ConfigOperations::addTargetChat(config, chatToAdd.toTargetChatT());
  return make_success_result();
}

Result< void > BusinessLogic::addChatToSource(Chat_t chatToAdd)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isModeConfig(config))
    return make_error_result("Mode is not config");
  if (CfgConds::isChatSource(config, chatToAdd.vkChatId))
    return make_error_result("Chat is already source");
  if (CfgConds::isVkChatIdInTargets(config, chatToAdd.vkChatId))
    return make_error_result("Chat is in targets");
  if (CfgConds::isSourceChatSet(config))
    return make_error_result("Source chat is already set");
  ConfigOperations::addSourceChat(config, chatToAdd.toSourceChatT());
  return make_success_result();
}

Result< void > BusinessLogic::addGodlike(VkUserId_t callerId, VkUserId_t newGodlike)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isUserGodlike(config, callerId))
    return make_error_result("Only godlike user can create another godlike");
  if (CfgConds::isUserGodlike(config, newGodlike))
    return make_error_result("User is already godlike");
  ConfigOperations::addGodlike(config, newGodlike);
  return make_success_result();
}

Result< void > BusinessLogic::removeTargetChatById(VkUserId_t callerId, ChatId_t chatId)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isUserGodlike(config, callerId))
    return make_error_result("Only godlike user can use this command");
  if (!CfgConds::isChatIdInTargets(config, chatId))
    return make_error_result("Chat is not target");
  ConfigOperations::removeTargetChatById(config, chatId);
  return make_success_result();
}

Result< void > BusinessLogic::removeTargetChatByVkChatId(VkChatId_t vkChatId)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isVkChatIdInTargets(config, vkChatId))
    return make_error_result("Chat is not target");
  ConfigOperations::removeTargetChatByVkChatId(config, vkChatId);
  return make_success_result();
}

Result< void > BusinessLogic::removeSourceChat(VkChatId_t callerVkChatId)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isSourceChatSet(config))
    return make_error_result("Source chat does not exist yet");
  if (!CfgConds::isChatSource(config, callerVkChatId))
    return make_error_result("Command not from source chat");
  ConfigOperations::removeSourceChat(config);
  return make_success_result();
}

Result< std::string > BusinessLogic::changeMode(VkUserId_t callerId)
{
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!CfgConds::isUserGodlike(config, callerId))
    return make_error_result("Only godlike user can use this command");
  if (!CfgConds::canChangeMode(config))
    return make_error_result("Some requirements were not met");
  ConfigOperations::changeMode(config);
  return make_success_result(ConfigOperations::getModeName(config));
}

int BusinessLogic::getPort()
{
  return ConfigHolder::getPort();
}

std::string BusinessLogic::getSecretString()
{
  return ConfigHolder::getSecretString();
}

BusinessLogic::SourceChat_t BusinessLogic::Chat_t::toSourceChatT()
{
  SourceChat_t chat;
  chat.title = this->title;
  chat.vkChatId = this->vkChatId;
  return chat;
}

BusinessLogic::TargetChat_t BusinessLogic::Chat_t::toTargetChatT()
{
  TargetChat_t chat;
  chat.title = this->title;
  chat.vkChatId = this->vkChatId;
  chat.chatId = this->chatId;
  return chat;
}
