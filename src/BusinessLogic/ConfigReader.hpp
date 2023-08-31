#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <shared_mutex>
#include <mutex>
#include <jsoncpp/json/json.h>
#include "../BotTypes/TargetsTable.hpp"
#include "../JsonUtils.hpp"

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
    std::string secretString;
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
    static void init(const std::string &configPath);
    static void updateConfigFile();
    static ReadOnlyConfig getReadOnlyConfig();
    static ReadWriteConfig getReadWriteConfig();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    static Mode getMode();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    static const std::string &getToken();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    static const std::string &getV();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    static int getPort();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    static int getGroupId();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    static const std::string &getSecretString();
    static bool hasBaseUrl();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    /// @note Probably you'd like to call hasBaseUrl() before
    static const std::string &getBaseUrl();
    /// @return True if given mode is valid to set now. False otherwise
    static bool isModeValid(const Config &cfg, Mode mode);

  private:
    static Config config_;
    static std::shared_mutex mut_;
    static std::string configPath_;

    struct ReadOnlyConfig
    {
      ReadOnlyConfig() : config(ConfigHolder::config_), lock(mut_) { }
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
      ReadWriteConfig() : config(ConfigHolder::config_), lock(mut_) { }
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
