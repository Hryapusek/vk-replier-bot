#define BOOST_LOG_DYN_LINK 1
#include "ConfigReader.hpp"
#include <fstream>
#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>
#include <functional>

namespace _details
{
  using json_crefw = std::reference_wrapper< const Json::Value >;
  using namespace config;
  void logAndThrow(const std::string &message)
  {
    BOOST_LOG_TRIVIAL(fatal) << message;
    throw std::logic_error(message);
  }

  // TODO update it
  void checkConfigValidity(Config &config)
  {
    if (config.mode == Mode::WORK && (!config.sourceChat || config.targetsTable.empty()))
      logAndThrow("source chat or target chats are missed for WORK mode");
  }

  void checkGeneralNecessaryFields(json_crefw root)
  {
    for (const auto &field : ConfigHolder::generalNecessaryFields)
    {
      if (!root.get().isMember(field))
        logAndThrow("Field " + field + " not found");
    }
  }

  Chat extractChat(const Json::Value &root)
  {
    if (root.isMember("title"))
      return Chat{ root["peer_id"].asInt(), root["title"].asString() };
    else
      return Chat{ root["peer_id"].asInt() };
  }

  void extractTargetChats(json_crefw root, Config &config)
  {
    if (root.get().empty())
    {
      config.targetsTable = TargetsTable();
      return;
    }
    TargetsTable table;
    int count = 0;
    for (const auto &obj : root.get())
    {
      ++count;
      if (!obj.isMember("num") || !obj.isMember("peer_id"))
        logAndThrow("Incorrect object in target chats. Object count " + std::to_string(count));
      Chat target = extractChat(obj);
      table.insert(obj["num"].asInt(), target);
    }
    config.targetsTable = table;
  }

  std::vector< int > extractIntVector(json_crefw root)
  {
    std::vector< int > nums;
    for (const auto &obj : root.get())
    {
      nums.push_back(obj.asInt());
    }
    return nums;
  }

  //If root is empty - no changes will be applied to field.
  void extractIntVectorIntoField(json_crefw root, std::optional< std::vector< int > > &field)
  {
    if (root.get().empty())
      return;
    field = extractIntVector(root);
  }

  void parseConfigJson(json_crefw root, Config &config)
  {
    checkGeneralNecessaryFields(root);
    if (root.get()["mode"].asString() == "config")
      config.mode = Mode::CONFIG;
    else if (root.get()["mode"].asString() == "work")
      config.mode = Mode::WORK;
    else
      logAndThrow("Incorrect mode found in config");
    config.token = root.get()["token"].asString();
    config.v = root.get()["v"].asString();
    config.secret_string = root.get()["secret_string"].asString();
    config.port = root.get()["port"].asInt();
    extractTargetChats(root.get()["target_chats"], config);
    if (root.get().isMember("source_chat"))
      config.sourceChat = extractChat(root.get()["source_chat"]);
    if (root.get().isMember("base_url"))
      config.baseUrl = root.get()["base_url"].asString();
    extractIntVectorIntoField(root.get()["status_checkers"], config.statusCheckersIds);
    extractIntVectorIntoField(root.get()["godlike_ids"], config.godlikeIds);
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
  const std::vector< std::string > ConfigHolder::generalNecessaryFields = {
    "mode", "token", "secret_string", "port", "v"
  };
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
    Config tempConfig;
    parseConfigJson(root, tempConfig);
    checkConfigValidity(tempConfig);
    target_ids = getTargetIdsString(tempConfig);
    config = std::move(tempConfig);
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

  Chat ConfigHolder::getSourceChat()
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
