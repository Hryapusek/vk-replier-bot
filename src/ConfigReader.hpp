#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <array>
#include <shared_mutex>
#include <jsoncpp/json/json.h>
#include "TargetsTable.hpp"
#include "JsonUtils.hpp"

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
    std::string v;
    std::string secret_string;
    int port;
    std::optional< std::string > baseUrl;
    TargetsTable targetsTable;
    std::optional< SourceChat > sourceChat;
    std::optional< std::vector< int > > statusCheckersIds;
    std::optional< std::vector< int > > godlikeIds;
  };

  class ConfigHolder
  {
    struct ReadOnlyConfig;
    struct ReadWriteConfig;

  public:
    /// @brief Init function
    /// @throws std::logic_error - Fatal error occured
    /// @throws Json::Exception - Fatal error occured
    static void readConfigFromFile(const std::string &configName);
    static void updateConfigFile();
    //TODO get Token v secret_string port
    static ReadOnlyConfig getReadOnlyConfig();
    static ReadWriteConfig getReadWriteConfig();
    /// @return Target ids from config in format id1, id2, ...
    /// @note Use this if you can instead of direct config use.
    /// String calculated only once on startup.
    /// Use this only in work mode.
    static const std::string &getTargetIds();
    /// @note Use this only in work mode.
    /// @throw std::bad_optional_access
    static SourceChat getSourceChat();
    /// @note Use this only in work mode.
    static Mode getMode();
    /// @note Use this only in work mode.
    static const TargetsTable &getTargetsTable();

  private:
    static Config config;
    static std::shared_mutex mut;
    static std::string configName;
    static std::string target_ids;

    struct ReadOnlyConfig
    {
      ReadOnlyConfig() : config(ConfigHolder::config), lock(mut) { }
      ReadOnlyConfig(const ReadOnlyConfig &) = delete;
      ReadOnlyConfig(ReadOnlyConfig &&) = delete;
      ReadOnlyConfig &operator=(const ReadOnlyConfig &) = delete;
      ReadOnlyConfig &operator=(ReadOnlyConfig &&) = delete;
      const Config &config;
    private:
      std::shared_lock< std::shared_mutex > lock;
    };

    struct ReadWriteConfig
    {
      ReadWriteConfig() : config(ConfigHolder::config), lock(mut) { }
      ReadWriteConfig(const ReadWriteConfig &) = delete;
      ReadWriteConfig(ReadWriteConfig &&) = delete;
      ReadWriteConfig &operator=(const ReadWriteConfig &) = delete;
      ReadWriteConfig &operator=(ReadWriteConfig &&) = delete;
      ~ReadWriteConfig() { updateConfigFile(); };
      Config &config;
    private:
      std::unique_lock< std::shared_mutex > lock;
    };
  };
}

#endif
