#include "BusinessLogic.hpp"
#include "../Logging/Logger.hpp"
#include "ConfigHolder.hpp"
#include "../VkApi/Requests/Init.hpp"
#include "Conditions.hpp"
#include "ConfigTypes/Chats.hpp"
#include "ConfigOperations.hpp"

const std::string BusinessLogic::configPath = "config.json";
const std::string BusinessLogic::logFilePath = "logfile.log";

using namespace config;

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

Result<std::string> BusinessLogic::getTagAllString(VkChatId_t callerChatId)
{
  using namespace conditions;
  auto configWrapper = ConfigHolder::getReadOnlyConfig();
  auto &config = configWrapper.config;
  if (!isModeWork(config))
    return make_error_result("Mode is not work");
  if (!isChatSource(config, callerChatId))
    return make_error_result("Chat is not source");
  if (!isAnyTargetChatPresent(config))
    return make_error_result("No target chats were found");
  return ConfigOperations::getTargetsString(config);
}

Result<void> BusinessLogic::addChatToTaget(Chat_t chatToAdd)
{
  using namespace conditions;
  auto configWrapper = ConfigHolder::getReadWriteConfig();
  auto &config = configWrapper.config;
  if (!isModeConfig(config))
    return make_error_result("Mode is not config");
  if (isChatSource(config, chatToAdd.vkChatId))
    return make_error_result("Chat is source");
  if (isChatInTargets(config, chatToAdd.vkChatId))
    return make_error_result("Chat is already target");
  if (chatToAdd.chatId && isChatIdBusy(config, *chatToAdd.chatId))
    return make_error_result("Requested id is already busy");
  ConfigOperations::addTargetChat(config, chatToAdd.toTargetChatT());
  return make_success_result();
}

BusinessLogic::SourceChat_t BusinessLogic::Chat_t::toSourceChatT()
{
  // TODO make realization
  return SourceChat_t();
}

BusinessLogic::TargetChat_t BusinessLogic::Chat_t::toTargetChatT()
{
  // TODO make realization
  return TargetChat_t();
}
