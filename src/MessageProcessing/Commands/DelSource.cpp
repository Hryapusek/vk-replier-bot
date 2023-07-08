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
  void delSource(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "delSource";
    if (!checkMode(config::Mode::CONFIG, commandName, "Can not perform this command"))
      return;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    str_cref errorMessage = "Current chat is not source. PeerId: " + std::to_string(message.getPeerId()) + "Skipping";
    if (!checkIfCommandFromChat(message, commandName, "Command not from chat. Skipping message")
        || !checkIfChatIsSource(message.getPeerId(), req, commandName, errorMessage))
      return;
    ConfigHolder::getReadWriteConfig().config.sourceChat = std::nullopt;
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully deleted source " << message.getPeerId();
    req.message("Successfully deleted this chat from source!").execute();
    return;
  }
}
