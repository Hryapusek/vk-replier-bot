#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <shared_mutex>
#include <jsoncpp/json/json.h>
#include "BotTypes/TargetsTable.hpp"
#include "JsonUtils.hpp"

namespace config
{
  enum Mode
  {
    WORK,
    CONFIG
  };

  const std::map<Mode, std::string> modeToString
  {
    { Mode::WORK, "WORK" },
    { Mode::CONFIG, "CONFIG" },
  };

  struct Config
  {
    Mode mode;
    std::string token;
    std::string v;
    std::string secret_string;
    int port;
    int groupID;
    std::optional< std::string > baseUrl;
    TargetsTable targetsTable;
    std::optional< SourceChat > sourceChat;
    std::vector< int > godlikeIds;
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
    static ReadOnlyConfig getReadOnlyConfig();
    static ReadWriteConfig getReadWriteConfig();
    /// @return Target ids string from config in format "id1, id2, ..."
    /// @note String calculated only once on startup.
    /// It is supposed that target id's are constant after startup.
    static const std::string &getTargetIds();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    static Mode getMode();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    static const std::string &getToken();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    static const std::string &getV();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    static int getPort();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    static int getGroupId();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    static const std::string &getSecretString();
    static bool hasBaseUrl();
    /// @brief Direct access to config field since
    /// it's constant and necessary in config file.
    /// @note Probably you'd like to call hasBaseUrl() before
    static const std::string &getBaseUrl();
    static bool isModeValid(Mode mode);

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
