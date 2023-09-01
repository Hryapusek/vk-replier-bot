#include "RegTargetCommand.hpp"
#include "../NewUtils.hpp"
#include "../ArgsExtractor.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::commands
{
  void RegTargetCommand::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Can not add target chat. "s;
    if (!msg.fromChat())
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
      return;
    }
    BusinessLogic::Chat_t chatToAdd;
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText(), triggerWords_), msg.getText().end(), true);
    if (argsExtractor.hasInt())
    {
      auto res = argsExtractor.extractInt();
      if (!res)
        return utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      chatToAdd.chatId = res.getObject();
    }
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
    auto res = BusinessLogic::addChatToTaget(std::move(chatToAdd));
    if (!res)
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      return;
    }
  }
}
