#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <shared_mutex>
#include <mutex>
#include <jsoncpp/json/json.h>
#include "ConfigTypes/TargetsTable.hpp"
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

  class Config
  {
    friend class ConfigHolder;
  private:
    Mode mode;
    std::string token;
    std::string v;
    std::string secretString;
    int port;
    int groupID;
    std::optional< std::string > baseUrl;
    TargetsTable targetsTable;
    std::optional< types::SourceChat_t > sourceChat;
    std::vector< int > godlikeIds;
  };

  /**
   * @note Init should be called before usage!
   */
  class ConfigHolder
  {
    struct ReadOnlyConfig;
    struct ReadWriteConfig;
    friend struct ReadOnlyConfig;
    friend struct ReadWriteConfig;
    friend class ConfigOperations;

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
      ReadOnlyConfig(const ReadOnlyConfig &) = delete;
      ReadOnlyConfig(ReadOnlyConfig &&) = delete;
      ReadOnlyConfig &operator=(const ReadOnlyConfig &) = delete;
      ReadOnlyConfig &operator=(ReadOnlyConfig &&) = delete;
      const Config &config;
    private:
      std::shared_lock< std::shared_mutex > lock;
      ReadOnlyConfig() : config(ConfigHolder::config_), lock(mut_) { }
    };

    struct ReadWriteConfig
    {
      ReadWriteConfig(const ReadWriteConfig &) = delete;
      ReadWriteConfig(ReadWriteConfig &&) = delete;
      ReadWriteConfig &operator=(const ReadWriteConfig &) = delete;
      ReadWriteConfig &operator=(ReadWriteConfig &&) = delete;
      ~ReadWriteConfig() { updateConfigFile(); };
      Config &config;
    private:
      std::unique_lock< std::shared_mutex > lock;
      ReadWriteConfig() : config(ConfigHolder::config_), lock(mut_) { }
    };
  };
}

#endif
