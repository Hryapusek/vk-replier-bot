#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <shared_mutex>
#include "TargetsTable.hpp"

namespace config
{
  enum Mode
  {
    WORK,
    CONFIG
  };

  struct Config
  {
    Mode mode;
    std::string token;
    std::optional< TargetsTable > targetsTable;
    std::optional< int > sourceChatId;
    std::optional< std::vector< int > > statusCheckersId;
    std::string secret_string;
  };

  class ConfigHolder
  {
    struct ReadOnlyConfig;
    struct ReadWriteConfig;

  public:
    static void readConfigFromFile(const std::string &fileName);
    static ReadOnlyConfig getReadOnlyConfig();
    static ReadWriteConfig getReadWriteConfig();
    /// @return Target ids from config in format id1, id2, ... 
    /// @note Use this if you can instead of direct config use.
    /// String calculated only once on startup.
    static const std::string &getTargetIds();

  private:
    static Config config;
    static std::shared_mutex mut;
    static const std::string target_ids;

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
}

#endif
