#include "BusinessLogic.hpp"
#include "../Logging/Logger.hpp"
#include "ConfigReader.hpp"
#include "../VkApi/Requests/Init.hpp"
#include "Conditions.hpp"

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

Result<std::string> BusinessLogic::getTagAllString(UserId_t callerId, ChatId_t callerChatId)
{
  using namespace conditions;
  auto configWrapper = ConfigHolder::getReadOnlyConfig();
  auto &config = configWrapper.config;
  if (!isModeWork(config))
    return make_error_result<std::string>("Mode is not work");
  if (!isChatSource(config, callerChatId))
    return make_error_result<std::string>("Chat is not source");
  if (!isAnyTargetChatPresent(config))
    return make_error_result<std::string>("No target chats were found");
  return config.targetsTable.getTargetIdsString();
}
