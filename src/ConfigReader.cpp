#include "ConfigReader.hpp"

Config ConfigHolder::config = Config();
std::shared_mutex ConfigHolder::mut = std::shared_mutex();


ConfigHolder::ReadOnlyConfig ConfigHolder::getReadOnlyConfig()
{
  return ReadOnlyConfig();
}

ConfigHolder::ReadWriteConfig ConfigHolder::getReadWriteConfig()
{
  return ReadWriteConfig();
}
