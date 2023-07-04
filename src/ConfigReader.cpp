#define BOOST_LOG_DYN_LINK 1
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
    JsonFieldT< std::string >("base_url", !necessary, std::bind(doIfFieldIncorrect, "base_url")),
    JsonFieldVT("target_chats", Json::ValueType::arrayValue, !necessary, std::bind(doIfFieldIncorrect, "target_chats")),
    JsonFieldVT("source_chat", Json::ValueType::objectValue, !necessary, std::bind(doIfFieldIncorrect, "source_chat")),
    JsonFieldVT("status_checkers", Json::ValueType::arrayValue, !necessary, std::bind(doIfFieldIncorrect, "status_checkers")),
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
    if (config.mode == Mode::WORK && (!config.sourceChat || config.targetsTable.empty()))
      logAndThrow("source chat or target chats are missed for WORK mode");
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
      JsonFieldT<int>("num", true, std::bind(doIfFieldIncorrect, "num")),
      JsonFieldT<int>("peer_id", true, std::bind(doIfFieldIncorrect, "peer_id")),
      JsonFieldT<int>("title", false, std::bind(doIfFieldIncorrect, "title")),
    };
    checkJsonFields(root, fields);
    if (root.isMember("title"))
      return TargetChat{ root["num"].asInt(), root["peer_id"].asInt(), root["title"].asString() }
    ;
    else
      return TargetChat{ root["num"].asInt(), root["peer_id"].asInt() }
    ;
  }

  SourceChat sourceChatFromJson(const Json::Value &root)
  {
    static const auto fields = std::tuple{
      JsonFieldT<int>("peer_id", necessary, std::bind(doIfFieldIncorrect, "peer_id")),
      JsonFieldT<int>("title", !necessary, std::bind(doIfFieldIncorrect, "title")),
    };
    checkJsonFields(root, fields);
    if (root.isMember("title"))
      return SourceChat{ root["peer_id"].asInt(), root["title"].asString() }
    ;
    else
      return SourceChat{ root["peer_id"].asInt() }
    ;
  }

  TargetsTable targetChatsFromJson(json_crefwrp root)
  {
    if (root.get().empty())
      return TargetsTable();
    TargetsTable table;
    for (const auto &obj : root.get())
    {
      TargetChat target = targetChatFromJson(obj);
      table.insert(target.num, target);
    }
    return table;
  }

  std::vector< int > intVectorFromJson(json_crefwrp root)
  {
    std::vector< int > nums;
    for (const auto &obj : root.get())
    {
      nums.push_back(obj.asInt());
    }
    return nums;
  }

  Json::Value intVectorToJson(std::vector< int > nums)
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
    config.targetsTable = targetChatsFromJson(root["target_chats"]);
    if (root.isMember("source_chat"))
      config.sourceChat = sourceChatFromJson(root["source_chat"]);
    if (root.isMember("base_url"))
      config.baseUrl = root["base_url"].asString();
    if (root.isMember("status_checkers") && !root["status_checkers"].empty())
      config.statusCheckersIds = intVectorFromJson(root["status_checkers"]);
    if (root.isMember("godlike_ids") && !root["godlike_ids"].empty())
      config.godlikeIds = intVectorFromJson(root["godlike_ids"]);
    return config;
  }

  std::string getTargetIdsString(Config &config)
  {
    std::string target_ids;
    for (const auto &[unused, target] : config.targetsTable.get())
    {
      target_ids += target.peer_id + ',';
    }
    if (!target_ids.empty())
      target_ids.pop_back();
    return target_ids;
  }
}

namespace config
{
  Config ConfigHolder::config = Config();
  std::shared_mutex ConfigHolder::mut = std::shared_mutex();
  std::string ConfigHolder::target_ids = std::string();
  std::string ConfigHolder::configName = std::string();

  using namespace _details;

  void ConfigHolder::readConfigFromFile(const std::string &configName)
  {
    std::ifstream configFile(configName);
    if (!configFile.is_open())
      logAndThrow("Can not open config file " + configName);
    ConfigHolder::configName = configName;
    Json::Value root;
    Json::CharReaderBuilder builder;
    if (!parseFromStream(builder, configFile, &root, nullptr))
      logAndThrow("Error while parsing json file");
    Config tempConfig = parseConfigJson(root);
    checkConfigValidity(tempConfig);
    target_ids = getTargetIdsString(tempConfig);
    config = std::move(tempConfig);
  }

  void ConfigHolder::updateConfigFile()
  {
    std::ofstream out(configName);
    if (!out.is_open())
      return;
    Json::Value configJson;
    if (config.mode = Mode::CONFIG)
      configJson["mode"] = "config";
    else if (config.mode = Mode::WORK)
      configJson["mode"] = "work";
    configJson["token"] = config.token;
    configJson["v"] = config.v;
    configJson["secret_string"] = config.secret_string;
    configJson["port"] = config.port;
    configJson["target_chats"] = targetChatsToJson(config);
    if (config.sourceChat)
      configJson["source_chat"] = sourceChatToJson(config.sourceChat.value());
    if (config.baseUrl)
      configJson["base_url"] = config.baseUrl.value();
    if (config.statusCheckersIds)
      configJson["status_checkers"] = intVectorToJson(config.statusCheckersIds.value());
    if (config.godlikeIds)
      configJson["godlike_ids"] = intVectorToJson(config.godlikeIds.value());
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

  const std::string &ConfigHolder::getTargetIds()
  {
    return target_ids;
  }

  SourceChat ConfigHolder::getSourceChat()
  {
    return config.sourceChat.value();
  }

  Mode ConfigHolder::getMode()
  {
    return config.mode;
  }

  const TargetsTable &ConfigHolder::getTargetsTable()
  {
    return config.targetsTable;
  }
}
