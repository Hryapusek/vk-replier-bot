#include "ConfigReader.hpp"
#include <fstream>
#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>
#include <functional>

namespace _details
{
  using json_cref = const Json::Value &;
  using json_crefwrp = std::reference_wrapper< const Json::Value >;
  using namespace config;

  void doIfFieldIncorrect(std::string fieldName);
  void logAndThrow(const std::string &message);

  const bool necessary = true;

  const std::tuple configFields{
    JsonFieldT< std::string >("mode", necessary, std::bind(doIfFieldIncorrect, "mode")),
    JsonFieldT< std::string >("token", necessary, std::bind(doIfFieldIncorrect, "token")),
    JsonFieldT< std::string >("v", necessary, std::bind(doIfFieldIncorrect, "v")),
    JsonFieldT< std::string >("secret_string", necessary, std::bind(doIfFieldIncorrect, "secret_string")),
    JsonFieldT< int >("port", necessary, std::bind(doIfFieldIncorrect, "port")),
    JsonFieldT< int >("group_id", necessary, std::bind(doIfFieldIncorrect, "group_id")),
    JsonFieldT< std::string >("base_url", !necessary, std::bind(doIfFieldIncorrect, "base_url")),
    JsonFieldVT("target_chats", Json::ValueType::arrayValue, !necessary, std::bind(doIfFieldIncorrect, "target_chats")),
    JsonFieldVT("source_chat", Json::ValueType::objectValue, !necessary, std::bind(doIfFieldIncorrect, "source_chat")),
    JsonFieldVT("godlike_ids", Json::ValueType::arrayValue, !necessary, std::bind(doIfFieldIncorrect, "godlike_ids")),
  };

  void doIfFieldIncorrect(std::string fieldName)
  {
    logAndThrow(fieldName + " - field was not found or it's type incorrect in ConfigReader.cpp");
  }

  void logAndThrow(const std::string &message)
  {
    BOOST_LOG_TRIVIAL(fatal) << message;
    throw std::logic_error(message);
  }

  void checkConfigValidity(Config &config)
  {
    if (!ConfigHolder::isModeValid(config, config.mode))
      logAndThrow("Config mode and fields can not work together");
  }

  Json::Value targetChatToJson(const TargetChat &chat)
  {
    Json::Value chatJson;
    chatJson["num"] = chat.num;
    chatJson["peer_id"] = chat.peer_id;
    if (chat.title)
      chatJson["title"] = chat.title.value();
    return chatJson;
  }

  Json::Value sourceChatToJson(const SourceChat &chat)
  {
    Json::Value chatJson;
    chatJson["peer_id"] = chat.peer_id;
    if (chat.title)
      chatJson["title"] = chat.title.value();
    return chatJson;
  }

  Json::Value targetChatsToJson(Config &config)
  {
    Json::Value targetChats(Json::arrayValue);
    for (const auto &pair : config.targetsTable.get())
      targetChats.append(targetChatToJson(pair.second));
    return targetChats;
  }

  TargetChat targetChatFromJson(const Json::Value &root)
  {
    static const auto fields = std::tuple{
      JsonFieldT< int >("num", true, std::bind(doIfFieldIncorrect, "num")),
      JsonFieldT< int >("peer_id", true, std::bind(doIfFieldIncorrect, "peer_id")),
      JsonFieldT< std::string >("title", false, std::bind(doIfFieldIncorrect, "title")),
    };
    checkJsonFields(root, fields);
    if (root.isMember("title"))
      return TargetChat{ root["num"].asInt(), root["peer_id"].asInt(), root["title"].asString() };
    else
      return TargetChat{ root["num"].asInt(), root["peer_id"].asInt(), std::nullopt };
  }

  SourceChat sourceChatFromJson(const Json::Value &root)
  {
    static const auto fields = std::tuple{
      JsonFieldT< int >("peer_id", necessary, std::bind(doIfFieldIncorrect, "peer_id")),
      JsonFieldT< std::string >("title", !necessary, std::bind(doIfFieldIncorrect, "title")),
    };
    checkJsonFields(root, fields);

    if (root.isMember("title"))
      return SourceChat{ root["peer_id"].asInt(), root["title"].asString() };
    else
      return SourceChat{ root["peer_id"].asInt(), std::nullopt };
  }

  TargetsTable targetChatsFromJson(json_crefwrp root)
  {
    if (root.get().empty())
      return TargetsTable();
    TargetsTable table;
    for (const auto &obj : root.get())
    {
      TargetChat target = targetChatFromJson(obj);
      table.insert(target);
    }
    return table;
  }

  std::vector< int > intVectorFromJson(json_crefwrp root)
  {
    std::vector< int > nums;
    for (const auto &obj : root.get())
      nums.push_back(obj.asInt());
    return nums;
  }

  Json::Value intVectorToJson(const std::vector< int > &nums)
  {
    Json::Value arr(Json::arrayValue);
    for (const auto num : nums)
      arr.append(num);
    return arr;
  }

  Config parseConfigJson(json_cref root)
  {
    checkJsonFields(root, configFields);
    Config config;
    if (root["mode"].asString() == "config")
      config.mode = Mode::CONFIG;
    else if (root["mode"].asString() == "work")
      config.mode = Mode::WORK;
    else
      logAndThrow("Incorrect mode found in config");
    config.token = root["token"].asString();
    config.v = root["v"].asString();
    config.secret_string = root["secret_string"].asString();
    config.port = root["port"].asInt();
    config.groupID = root["group_id"].asInt();
    config.targetsTable = targetChatsFromJson(root["target_chats"]);
    if (root.isMember("source_chat"))
      config.sourceChat = sourceChatFromJson(root["source_chat"]);
    if (root.isMember("base_url"))
      config.baseUrl = root["base_url"].asString();
    if (root.isMember("godlike_ids"))
      config.godlikeIds = intVectorFromJson(root["godlike_ids"]);
    return config;
  }
}

namespace config
{
  Config ConfigHolder::config_ = Config();
  std::shared_mutex ConfigHolder::mut_ = std::shared_mutex();
  std::string ConfigHolder::configPath_ = std::string();

  using namespace _details;

  void ConfigHolder::init(const std::string &configPath)
  {
    std::ifstream configFile(configPath);
    if (!configFile.is_open())
      logAndThrow("Can not open config file " + configPath);
    ConfigHolder::configPath_ = configPath;
    Json::Value root;
    Json::CharReaderBuilder builder;
    if (!parseFromStream(builder, configFile, &root, nullptr))
      logAndThrow("Error while parsing json file");
    Config tempConfig = parseConfigJson(root);
    checkConfigValidity(tempConfig);
    config_ = std::move(tempConfig);
  }

  void ConfigHolder::updateConfigFile()
  {
    std::ofstream out(configPath_);
    if (!out.is_open())
      return;
    Json::Value configJson;
    if (config_.mode == Mode::CONFIG)
      configJson["mode"] = "config";
    else if (config_.mode == Mode::WORK)
      configJson["mode"] = "work";
    configJson["token"] = config_.token;
    configJson["v"] = config_.v;
    configJson["secret_string"] = config_.secret_string;
    configJson["port"] = config_.port;
    configJson["group_id"] = config_.groupID;
    configJson["target_chats"] = targetChatsToJson(config_);
    if (config_.sourceChat)
      configJson["source_chat"] = sourceChatToJson(config_.sourceChat.value());
    if (config_.baseUrl)
      configJson["base_url"] = config_.baseUrl.value();
    if (!config_.godlikeIds.empty())
      configJson["godlike_ids"] = intVectorToJson(config_.godlikeIds);
    Json::StreamWriterBuilder builder;
    const std::unique_ptr< Json::StreamWriter > writer(builder.newStreamWriter());
    writer->write(configJson, &out);
  }

  ConfigHolder::ReadOnlyConfig ConfigHolder::getReadOnlyConfig()
  {
    return ReadOnlyConfig();
  }

  ConfigHolder::ReadWriteConfig ConfigHolder::getReadWriteConfig()
  {
    return ReadWriteConfig();
  }

  Mode ConfigHolder::getMode()
  {
    return config_.mode;
  }

  const std::string &ConfigHolder::getToken()
  {
    return config_.token;
  }

  const std::string &ConfigHolder::getV()
  {
    return config_.v;
  }

  int ConfigHolder::getPort()
  {
    return config_.port;
  }

  int ConfigHolder::getGroupId()
  {
    return config_.groupID;
  }

  const std::string &ConfigHolder::getSecretString()
  {
    return config_.secret_string;
  }

  bool ConfigHolder::hasBaseUrl()
  {
    return config_.baseUrl.has_value();
  }

  const std::string &ConfigHolder::getBaseUrl()
  {
    return config_.baseUrl.value();
  }

  bool ConfigHolder::isModeValid(const Config &cfg, Mode mode)
  {
    switch (mode)
    {
    case Mode::CONFIG:
      return true;
    
    case Mode::WORK:
      return cfg.sourceChat && !cfg.targetsTable.empty();
    
    default:
      return false;
    }
  }
}
