#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <shared_mutex>
#include <mutex>
#include <jsoncpp/json/json.h>

#include "ConfigTypes/TargetsContainer.hpp"
#include "ConfigTypes/SimpleTypes.hpp"
#include "GeneralUtils/JsonUtils.hpp"

namespace config
{
  class Config
  {
    friend class ConfigOperations;
    friend class ConfigHolder;
    friend class ConfigConditions;
    using TargetsContainer = types::TargetsContainer;

  private:
    types::Mode mode;
    std::string token;
    std::string v;
    std::string secretString;
    int port;
    int groupID;
    std::string baseUrl;
    TargetsContainer targetChats;
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
    using json_cref = const Json::Value &;
    using json_crefwrp = std::reference_wrapper< const Json::Value >;

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
    static std::string getSecretString();
    static bool hasBaseUrl();
    /// @brief Direct access to config field.
    /// NOT thread-safe
    /// @note Probably you'd like to call hasBaseUrl() before
    static const std::string &getBaseUrl();

  private:
    static Config config_;
    static std::shared_mutex mut_;
    static std::string configPath_;

    struct ReadOnlyConfig
    {
      friend class ConfigHolder;
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
      friend class ConfigHolder;
      ReadWriteConfig(const ReadWriteConfig &) = delete;
      ReadWriteConfig(ReadWriteConfig &&) = delete;
      ReadWriteConfig &operator=(const ReadWriteConfig &) = delete;
      ReadWriteConfig &operator=(ReadWriteConfig &&) = delete;
      ~ReadWriteConfig() {
        updateConfigFile();
      };
      Config &config;
    private:
      std::unique_lock< std::shared_mutex > lock;
      ReadWriteConfig() : config(ConfigHolder::config_), lock(mut_) { }
    };

    static Config parseConfigJson(json_cref root);
    static void checkConfigValidity(Config &config);
  };
}

#endif
