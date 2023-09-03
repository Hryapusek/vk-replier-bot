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
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    if (!argsExtractor.eol())
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::changeMode(msg.getFromId());
    if (!res)
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::sendResponseMessage(msg.getPeerId(), "Successfully changed mode to " + res.getObject());
  }
}
