#include "RegSource.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace message_processing::commands
{
  using namespace utils;
  void regChecker(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "regChecker";
    if (!checkMode(config::Mode::CONFIG, commandName, "Can not use this command")
        || !checkIfFromDirect(message, commandName, "Not in direct. Skipping message"))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfChecker(message, req, commandName, "User is already checker"))
      return;
    {
      auto configWrap = ConfigHolder::getReadWriteConfig();
      auto &config = configWrap.config;
      config.statusCheckersIds.push_back(message.getPeerId());
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully registered checker " << message.getPeerId();
    req.message("Successfully registered checker!").execute();
    return;
  }
}
