#include "BusinessLogic.hpp"
#include "../Logging/Logger.hpp"
#include "ConfigReader.hpp"
#include "../VkApi/Requests/Init.hpp"

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
