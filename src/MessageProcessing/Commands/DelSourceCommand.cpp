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
    static const std::string textTriggerName = "DelSource";
    MsgUtils::logTextTriggerName(textTriggerName);
    if (!msg.fromChat())
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    if (!argsExtractor.eol())
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::removeSourceChat(msg.getPeerId());
    if (!res)
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::logAndSendResponseMessage(msg.getPeerId(), "Successfully deleted this chat from source!");
  }
}
