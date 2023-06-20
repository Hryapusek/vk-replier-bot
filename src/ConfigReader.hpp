#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <shared_mutex>
#include "BlocksTable.hpp"

enum Mode
{
  WORK,
  CONFIG
};

struct Config
{
  Mode mode;
  std::string token;
  std::optional< BlocksTable > blocksTable;
  std::optional< int > sourceChatId;
  std::optional< std::vector< int > > statusCheckersId;
  std::string secret_string;
};

class ConfigHolder
{
  struct ReadOnlyConfig;
  struct ReadWriteConfig;

public:
  void readConfigFromFile(const std::string &fileName);
  static ReadOnlyConfig getReadOnlyConfig();
  static ReadWriteConfig getReadWriteConfig();

private:
  static Config config;
  static std::shared_mutex mut;

  struct ReadOnlyConfig
  {
    ReadOnlyConfig() : config(config), lock(mut) { }
    const Config &config;
  private:
    std::shared_lock< std::shared_mutex > lock;
  };

  struct ReadWriteConfig
  {
    ReadWriteConfig() : config(config), lock(mut) { }
    Config &config;
  private:
    std::unique_lock< std::shared_mutex > lock;
  };
};

#endif
