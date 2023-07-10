#include "DelSource.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace message_processing::commands
{
  using namespace utils;
  void delSource(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "delSource";
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    str_cref errorMessage = "Can not perform command. Current chat is not source. PeerId: " + std::to_string(message.getPeerId()) + "Skipping";
    if (!checkIfCommandFromChat(message, commandName, "Can not perform command. Command not from chat.")
        || !checkMode(config::Mode::CONFIG, req, commandName, "Can not perform command")
        || !checkIfChatIsSource(message.getPeerId(), req, commandName, errorMessage))
      return;
    ConfigHolder::getReadWriteConfig().config.sourceChat = std::nullopt;
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully deleted source " << message.getPeerId();
    req.message("Successfully deleted this chat from source!").execute();
    return;
  }
}
