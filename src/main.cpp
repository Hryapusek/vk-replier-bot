#define BOOST_LOG_DYN_LINK 1
#include "Logging/Logger.hpp"
#include "ConfigReader.hpp"
#include "VkApi/Requests/Init.hpp"
#include "Server/Server.hpp"

// 1. Put server in separate file
// 2. Incapsulate logger
// 3. Put error messages in separate class
// 4. Parse Event NOT EventType
// TODO fix regex, add \n to whitespaces

// FIX - print TimeStamp, LineID, ProcessID, ThreadID
// OPTIMIZATION - config vectors to sets
// TODO - NAMESPACE REPLIER

void initVkRequests()
{
  using namespace vk::requests;
  using namespace config;
  if (ConfigHolder::hasBaseUrl())
    initRequests(ConfigHolder::getToken(), ConfigHolder::getV(), ConfigHolder::getBaseUrl());
  else
    initRequests(ConfigHolder::getToken(), ConfigHolder::getV());
}

void init()
{
  using namespace config;
  Logger::init("logfile.log");
  ConfigHolder::init("config.json");
  initVkRequests();
}

int main() {
  init();
  
  return 0;
}