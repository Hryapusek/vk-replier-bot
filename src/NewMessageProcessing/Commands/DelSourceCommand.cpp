#include "DelSourceCommand.hpp"
#include "../NewUtils.hpp"
#include "../ArgsExtractor.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void DelSourceCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not delete this chat from targets. "s;
    if (!msg.fromChat())
      return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText(), triggerWords_), msg.getText().end(), true);
    if (!argsExtractor.eol())
      return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::removeSourceChat(msg.getFromId());
    if (!res)
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      return;
    }
  }
}
