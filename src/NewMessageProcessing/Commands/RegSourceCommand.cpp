#include "RegSourceCommand.hpp"
#include "../NewUtils.hpp"
#include "../ArgsExtractor.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void RegSourceCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not add source chat. "s;
    if (!msg.fromChat())
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
      return;
    }
    BusinessLogic::Chat_t chatToAdd;
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText(), triggerWords_), msg.getText().end(), true);
    if (argsExtractor.hasQuotedString())
    {
      auto res = argsExtractor.extractQuotedString();
      if (!res)
      {
        return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      }
      chatToAdd.title = res.getObject();
    }
    if (!argsExtractor.eol())
      return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::addChatToSource(std::move(chatToAdd));
    if (!res)
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      return;
    }
  }
}
