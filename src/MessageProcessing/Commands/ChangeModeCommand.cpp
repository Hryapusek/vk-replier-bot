#include "ChangeModeCommand.hpp"
#include "MessageProcessing/MsgUtils.hpp"
#include "MessageProcessing/ArgsExtractor.hpp"
#include "BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void ChangeModeCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not change the mode. "s;
    static const std::string textTriggerName = "ChangeMode";
    MsgUtils::logTextTriggerName(textTriggerName);
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    if (!argsExtractor.eol())
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::changeMode(msg.getFromId());
    if (!res)
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::logAndSendResponseMessage(msg.getPeerId(), "Successfully changed mode to " + res.getObject());
  }
}
