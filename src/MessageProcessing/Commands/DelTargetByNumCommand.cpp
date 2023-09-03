#include "DelTargetByNumCommand.hpp"
#include "MessageProcessing/MsgUtils.hpp"
#include "MessageProcessing/ArgsExtractor.hpp"
#include "BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void DelTargetByNumCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not delete this target. "s;
    int targetChatId;
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    {
      auto res = argsExtractor.extractInt();
      if (!res)
        return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      targetChatId = res.getObject();
    }
    if (!argsExtractor.eol())
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::removeTargetChatById(msg.getFromId(), targetChatId);
    if (!res)
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::sendResponseMessage(msg.getPeerId(), "Successfully deleted chat " + std::to_string(targetChatId)  + " from target!");
  }
}
