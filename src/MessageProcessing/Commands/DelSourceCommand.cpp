#include "DelSourceCommand.hpp"
#include "MessageProcessing/MsgUtils.hpp"
#include "MessageProcessing/ArgsExtractor.hpp"
#include "BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void DelSourceCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not delete this chat from targets. "s;
    if (!msg.fromChat())
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    if (!argsExtractor.eol())
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::removeSourceChat(msg.getFromId());
    if (!res)
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::sendResponseMessage(msg.getPeerId(), "Successfully deleted this chat from source!");
  }
}
