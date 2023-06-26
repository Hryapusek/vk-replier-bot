#include "ConfigReader.hpp"
#include <fstream>
#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>
#include <functional>

namespace config
{
  Config ConfigHolder::config = Config();
  std::shared_mutex ConfigHolder::mut = std::shared_mutex();
  const std::vector< std::string > ConfigHolder::generalNecessaryFields = {
    "mode", "token", "secret_string", "port"
  };

  void logAndThrow(const std::string &message)
  {
    BOOST_LOG_TRIVIAL(fatal) << message;
    throw std::logic_error(message);
  }

  void checkGeneralNecessaryFields(std::reference_wrapper< const Json::Value > root)
  {
    for (const auto &field : ConfigHolder::generalNecessaryFields)
    {
      if (!root.get().isMember(field))
        logAndThrow("Field " + field + " not found");
    }
  }

  void extractTargetChats(std::reference_wrapper< const Json::Value > root, Config &config)
  {
    if (root.get().empty())
      return;
    TargetsTable table;
    int count = 0;
    for (const auto &obj : root.get())
    {
      ++count;
      if (!obj.isMember("num") || !obj.isMember("peer_id"))
        logAndThrow("Incorrect object in target chats. Object count " + std::to_string(count));
      Target target;
      if (obj.isMember("title"))
        target = { obj["peer_id"].asInt(), obj["title"].asString() };
      else
        target = { obj["peer_id"].asInt() };
      table.insert(obj["num"].asInt(), target);
    }
    config.targetsTable = table;
  }

  std::vector<int> extractIntVector(std::reference_wrapper< const Json::Value > root)
  {
    std::vector<int> nums;
    for (const auto &obj : root.get())
    {
      nums.push_back(obj.asInt());
    }
  }

  void extractIntVectorIntoField(std::reference_wrapper< const Json::Value > root, std::optional<std::vector<int>> &field)
  {
    if (root.get().empty())
      return;
    field = extractIntVector(root);
  }

  void parseConfigJson(std::reference_wrapper< const Json::Value > root, Config &config)
  {
    checkGeneralNecessaryFields(root);
    if (root.get()["mode"].asString() == "config")
      config.mode = Mode::CONFIG;
    else if (root.get()["mode"].asString() == "work")
      config.mode = Mode::WORK;
    else
      logAndThrow("Incorrect mode found in config");
    config.token = root.get()["token"].asString();
    config.secret_string = root.get()["secret_string"].asString();
    config.port = root.get()["port"].asInt();
    extractTargetChats(root.get()["target_chats"], config);
    if (!root.get()["source_chat"].empty())
      config.sourceChatId = root.get()["source_chat"].asInt();
    extractIntVectorIntoField(root.get()["status_checkers"], config.statusCheckersIds);
    extractIntVectorIntoField(root.get()["godlike_ids"], config.godlikeIds);
  }

  void ConfigHolder::readConfigFromFile(const std::string &fileName)
  {
    std::ifstream configFile(fileName);
    if (!configFile.is_open())
      logAndThrow("Can not open config file " + fileName);
    Json::Value root;
    Json::CharReaderBuilder builder;
    if (!parseFromStream(builder, configFile, &root, nullptr))
      logAndThrow("Error while parsing json file");
    parseConfigJson(root, config);
  }

  ConfigHolder::ReadOnlyConfig ConfigHolder::getReadOnlyConfig()
  {
    return ReadOnlyConfig();
  }

  ConfigHolder::ReadWriteConfig ConfigHolder::getReadWriteConfig()
  {
    return ReadWriteConfig();
  }
}
