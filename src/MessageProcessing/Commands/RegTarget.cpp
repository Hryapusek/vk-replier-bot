#include "RegTarget.hpp"
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

  bool checkIfNumberBusy(int num, MessagesSendRequest &req, str_cref command, str_cref errorMessage)
  {
    try
    {
      config::ConfigHolder::getReadOnlyConfig().config.targetsTable.at(num);
      logAndSendErrorMessage(req, command, errorMessage);
      return false;
    }
    catch (const std::out_of_range &e)
    {
      return true;
    }
  }
}

namespace message_processing::commands
{
  using namespace utils;
  void regTarget(const vk::objects::Message &message, size_t pos)
  {
    using namespace config;
    static str_cref commandName = "regTarget";
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkMode(Mode::CONFIG, commandName, "Can not perform")
        || !checkIfCommandFromChat(message, commandName, "Can not perform. Command not from chat. Skipping")
        || !checkIfChatIsNotSource(message.getPeerId(), req, commandName, "Can not perform. This chat is source.")
        || !checkIfPeerIdNotInTargetsTable(message, req, commandName, "Can not perform. This chat is present somewhere in the table"))
      return;
    std::optional< int > numOpt;
    std::string title;
    try
    {
      extractNumAndTitle(message.getText(), pos, numOpt, title);
    }
    catch (const std::logic_error &e)
    {
      logAndSendErrorMessage(req, commandName, "Can not perform. Incorrect number passed");
      return;
    }
    catch (const std::exception &e)
    {
      logAndSendErrorMessage(req, commandName, "Can not perform. Check if title is quoted both sides");
      return;
    }
    if (numOpt)
    {
      if (!checkIfNumberBusy(numOpt.value(), req, commandName, "Can not perform. Given num is already busy"))
        return;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.insert(TargetChat{ numOpt.value(), message.getPeerId(), title });
      if (!res)
      {
        logAndSendErrorMessage(req, commandName, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    else
    {
      TargetChat chat = TargetChat();
      chat.peer_id = message.getPeerId();
      chat.title = title;
      auto res = ConfigHolder::getReadWriteConfig().config.targetsTable.generateNumAndInsert(std::move(chat));
      if (!res)
      {
        logAndSendErrorMessage(req, commandName, "Unknown error. Insertion in targetsTable failed");
        return;
      }
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully registered target " << message.getPeerId();
    req.message("Successfully registered as target!").execute();
    return;
  }
}
