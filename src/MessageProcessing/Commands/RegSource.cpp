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
  bool checkIfSourceChatNotEmpty(MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    if (config::ConfigHolder::getReadOnlyConfig().config.sourceChat)
    {
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    return true;
  }
}

namespace message_processing::commands
{
  using namespace utils;
  void regSource(const Message &message, size_t pos)
  {
    using namespace config;
    static str_cref commandName = "regSource";
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkMode(Mode::CONFIG, commandName, "Can not perform command")
        || !checkIfSourceChatNotEmpty(req, commandName, "Delete current sourceChat first")
        || !checkIfCommandFromChat(message, commandName, "Can not perform. Command not from chat")
        || !checkIfChatIsNotSource(message.getPeerId(), req, commandName, "This chat is already source")
        || !checkIfPeerIdNotInTargetsTable(message, req, commandName, "This chat is present somewhere in the table"))
      return;
    std::string title = extractTitle(message.getText(), pos);
    ConfigHolder::getReadWriteConfig().config.sourceChat = SourceChat{ message.getPeerId(), title };
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully registered source " << message.getPeerId();
    req.message("Successfully registered as source!").execute();
    return;
  }
}
