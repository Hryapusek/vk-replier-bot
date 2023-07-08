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
}

namespace message_processing::commands
{
  using namespace utils;
  void delTarget(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "delTarget";
    if (!checkMode(config::Mode::CONFIG, commandName, "Can not use this command"))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfCommandFromChat(message, commandName, "Command not from chat. Skipping")
        || !checkIfPeerIdInTargetsTable(message, req, commandName, "Current chat is not target. PeerId: " + message.getPeerId()))
      return;
    if (!ConfigHolder::getReadWriteConfig().config.targetsTable.removeByPeerID(message.getPeerId()))
    {
      logAndSendErrorMessage(req, commandName, "Error while deleting this chat from targets");
      return;
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully deleted target " << message.getPeerId();
    req.message("Successfully deleted this chat from targets!").execute();
    return;
  }
}
