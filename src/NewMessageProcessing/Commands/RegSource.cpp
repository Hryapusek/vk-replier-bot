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
  bool checkIfSourceChatEmpty(MessagesSendRequest &req, str_cref command, str_cref errorMessage)
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
    if (!checkIfCommandFromChat(message, commandName, "Can not perform command. Command not from chat")
        || !checkMode(Mode::CONFIG, req, commandName, "Can not perform command")
        || !checkIfSourceChatEmpty(req, commandName, "Can not perform command. Delete current sourceChat first")
        || !checkIfChatIsNotSource(message.getPeerId(), req, commandName, "Can not perform command. This chat is already source")
        || !checkIfPeerIdNotInTargetsTable(message, req, commandName, "Can not perform command. This chat is present somewhere in the table"))
      return;
    std::string title;
    try
    {
      title = extractTitle(message.getText(), pos);
    }
    catch(const std::exception& e)
    {
      logAndSendErrorMessage(req, commandName, "Can not perform command. Check if title is quoted both sides");
      return;
    }
    ConfigHolder::getReadWriteConfig().config.sourceChat = SourceChat{ message.getPeerId(), title };
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully registered source " << message.getPeerId();
    req.message("Successfully registered as source!").execute();
    return;
  }
}
