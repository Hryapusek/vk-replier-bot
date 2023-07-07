#include "RegSource.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace
{
  using namespace message_processing;
  using namespace message_processing::utils;
  using str_cref = const std::string &;
  bool checkIfAlreadyChecker(const Message &message, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    auto configWrap = config::ConfigHolder::getReadOnlyConfig();
    auto checkersOpt = configWrap.config.statusCheckersIds;
    if (checkersOpt)
    {
      auto it = std::find(checkersOpt.value().begin(), checkersOpt.value().end(), message.getPeerId());
      if (it != checkersOpt.value().end())
      {
        logAndSendErrorMessage(req, command, errorMessage);
        return false;
      }
    }
    return true;
  }
}

namespace message_processing::commands
{
  using namespace utils;
  void regChecker(const Message &message)
  {
    using namespace config;
    if (!checkMode(config::Mode::CONFIG, "Can not use this command"))
      return;
    static str_cref commandName = "regChecker";
    if (!message.fromDirect())
    {
      BOOST_LOG_TRIVIAL(warning) << commandName << " not in direct. Skipping message";
      return;
    }
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfAlreadyChecker(message, req, commandName, "User is already checker"))
      return;
    {
      auto configWrap = ConfigHolder::getReadWriteConfig();
      auto &config = configWrap.config;
      if (!config.statusCheckersIds)
        config.statusCheckersIds = std::vector<int>();
      config.statusCheckersIds->push_back(message.getPeerId());
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully registered checker " << message.getPeerId();
    req.message("Successfully registered checker!").execute();
    return;
  }
}
