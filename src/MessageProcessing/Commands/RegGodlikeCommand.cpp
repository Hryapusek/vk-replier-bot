#include "RegGodlikeCommand.hpp"
#include "MessageProcessing/MsgUtils.hpp"
#include "MessageProcessing/ArgsExtractor.hpp"
#include "BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void RegGodlikeCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not register new godlike. "s;
    static const std::string textTriggerName = "RegGodlike";
    MsgUtils::logTextTriggerName(textTriggerName);
    int newGodlikeId;
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    {
      auto res = argsExtractor.extractInt();
      if (!res)
        return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      newGodlikeId = res.getObject();
    }
    if (!argsExtractor.eol())
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::addGodlike(msg.getFromId(), newGodlikeId);
    if (!res)
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::logAndSendResponseMessage(msg.getPeerId(), "Successfully registered godlike " + std::to_string(newGodlikeId) + "!");
  }
}
