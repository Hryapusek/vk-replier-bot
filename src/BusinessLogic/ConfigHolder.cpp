#include "ConfigHolder.hpp"
#include <fstream>
#include <jsoncpp/json/json.h>
#include <functional>

#include "Logging/Logger.hpp"
#include "ConfigConditions.hpp"
#include "ConfigTypes/SimpleTypes.hpp"

namespace
{
  using namespace config;
  using TargetChat_t = types::TargetChat_t;
  using SourceChat_t = types::SourceChat_t;
  using TargetsContainer = types::TargetsContainer;

  void doIfFieldIncorrect(std::string fieldName);
  void logAndThrow(const std::string &message);

  const bool necessary = true;
  const bool notNecessary = false;

  const std::tuple configFields{
    JsonFieldT< std::string >("mode", necessary, std::bind(doIfFieldIncorrect, "mode")),
    JsonFieldT< std::string >("token", necessary, std::bind(doIfFieldIncorrect, "token")),
    JsonFieldT< std::string >("v", necessary, std::bind(doIfFieldIncorrect, "v")),
    JsonFieldT< std::string >("secretString", necessary, std::bind(doIfFieldIncorrect, "secretString")),
    JsonFieldT< int >("port", necessary, std::bind(doIfFieldIncorrect, "port")),
    JsonFieldT< int >("groupId", necessary, std::bind(doIfFieldIncorrect, "groupId")),
    JsonFieldT< std::string >("baseUrl", notNecessary, std::bind(doIfFieldIncorrect, "baseUrl")),
    JsonFieldVT("targetChats", Json::ValueType::arrayValue, notNecessary, std::bind(doIfFieldIncorrect, "targetChats")),
    JsonFieldVT("sourceChat", Json::ValueType::objectValue, notNecessary, std::bind(doIfFieldIncorrect, "sourceChat")),
    JsonFieldVT("godlikeIds", Json::ValueType::arrayValue, notNecessary, std::bind(doIfFieldIncorrect, "godlikeIds")),
  };

  void doIfFieldIncorrect(std::string fieldName)
  {
    logAndThrow(fieldName + " - field was not found or it's type incorrect in ConfigReader.cpp");
  }

  void logAndThrow(const std::string &message)
  {
    Logger::log(Logger::ERROR, message);
    throw std::logic_error(message);
  }

  Json::Value targetChatToJson(const TargetChat_t &chat)
  {
    Json::Value chatJson;
    chatJson["chatId"] = *chat.chatId;
    chatJson["vkChatId"] = chat.vkChatId;
    if (!chat.title.empty())
      chatJson["title"] = chat.title;
    return chatJson;
  }

  Json::Value sourceChatToJson(const SourceChat_t &chat)
  {
    Json::Value chatJson;
    chatJson["vkChatId"] = chat.vkChatId;
    if (!chat.title.empty())
      chatJson["title"] = chat.title;
    return chatJson;
  }

  TargetChat_t targetChatFromJson(const Json::Value &root)
  {
    static const auto fields = std::tuple{
      JsonFieldT< int >("chatId", true, std::bind(doIfFieldIncorrect, "chatId")),
      JsonFieldT< int >("vkChatId", true, std::bind(doIfFieldIncorrect, "vkChatId")),
      JsonFieldT< std::string >("title", false, std::bind(doIfFieldIncorrect, "title")),
    };
    checkJsonFields(root, fields);
    TargetChat_t targetChat;
    targetChat.chatId = root["chatId"].asInt();
    targetChat.vkChatId = root["vkChatId"].asInt();
    if (root.isMember("title"))
      targetChat.title = root["title"].asString();
    return targetChat;
  }

  SourceChat_t sourceChatFromJson(const Json::Value &root)
  {
    static const auto fields = std::tuple{
      JsonFieldT< int >("vkChatId", necessary, std::bind(doIfFieldIncorrect, "vkChatId")),
      JsonFieldT< std::string >("title", notNecessary, std::bind(doIfFieldIncorrect, "title")),
    };
    checkJsonFields(root, fields);
    SourceChat_t sourceChat;
    sourceChat.vkChatId = root["vkChatId"].asInt();
    if (root.isMember("title"))
      sourceChat.title = root["title"].asString();
    return sourceChat;
  }

  TargetsContainer targetChatsFromJson(const Json::Value &root)
  {
    if (root.empty())
      return TargetsContainer();
    TargetsContainer table;
    for (const auto &obj : root)
    {
      TargetChat_t target = targetChatFromJson(obj);
      table.insert(target);
    }
    return table;
  }

  Json::Value targetChatsToJsonArray(const TargetsContainer &table)
  {
    Json::Value targetChats(Json::arrayValue);
    for (const auto &pair : table)
      targetChats.append(targetChatToJson(pair.second));
    return targetChats;
  }

  std::vector< int > intVectorFromJson(const Json::Value &root)
  {
    std::vector< int > nums;
    for (const auto &obj : root)
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

}

namespace config
{
  Config ConfigHolder::config_ = Config();
  std::shared_mutex ConfigHolder::mut_ = std::shared_mutex();
  std::string ConfigHolder::configPath_ = std::string();

  using namespace types;

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
    configJson["secretString"] = config_.secretString;
    configJson["port"] = config_.port;
    configJson["groupId"] = config_.groupID;
    configJson["targetChats"] = targetChatsToJsonArray(config_.targetChats);
    if (config_.sourceChat)
      configJson["sourceChat"] = sourceChatToJson(config_.sourceChat.value());
    if (!config_.baseUrl.empty())
      configJson["baseUrl"] = config_.baseUrl;
    if (!config_.godlikeIds.empty())
      configJson["godlikeIds"] = intVectorToJson(config_.godlikeIds);
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

  std::string ConfigHolder::getSecretString()
  {
    return config_.secretString;
  }

  bool ConfigHolder::hasBaseUrl()
  {
    return !config_.baseUrl.empty();
  }

  const std::string &ConfigHolder::getBaseUrl()
  {
    return config_.baseUrl;
  }

  Config ConfigHolder::parseConfigJson(json_cref root)
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
    config.secretString = root["secretString"].asString();
    config.port = root["port"].asInt();
    config.groupID = root["groupId"].asInt();
    config.targetChats = targetChatsFromJson(root["targetChats"]);
    if (root.isMember("sourceChat"))
      config.sourceChat = sourceChatFromJson(root["sourceChat"]);
    if (root.isMember("baseUrl"))
      config.baseUrl = root["baseUrl"].asString();
    if (root.isMember("godlikeIds"))
      config.godlikeIds = intVectorFromJson(root["godlikeIds"]);
    return config;
  }

  void ConfigHolder::checkConfigValidity(Config &config)
  {
    if (!ConfigConditions::isModeValid(config, config.mode))
      logAndThrow("Config mode and fields can not work together");
  }
}
