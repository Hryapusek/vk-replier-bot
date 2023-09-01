#include "RegGodlikeCommand.hpp"
#include "../NewUtils.hpp"
#include "../ArgsExtractor.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void RegGodlikeCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not add source chat. "s;
    if (!msg.fromChat())
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
      return;
    }
    int newGodlikeId;
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText(), triggerWords_), msg.getText().end(), true);
    if (argsExtractor.hasInt())
    {
      auto res = argsExtractor.extractInt();
      if (!res)
        return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      newGodlikeId = res.getObject();
    }
    if (!argsExtractor.eol())
      return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::addGodlike(msg.getPeerId(), newGodlikeId);
    if (!res)
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      return;
    }
  }
}
