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
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Command not from chat");
    BusinessLogic::Chat_t chatToAdd;
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    if (argsExtractor.hasNum())
    {
      auto res = argsExtractor.extractInt();
      if (!res)
        return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      chatToAdd.chatId = res.getObject();
    }
    if (argsExtractor.hasQuotedString())
    {
      auto res = argsExtractor.extractQuotedString();
      if (!res)
        return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      chatToAdd.title = res.getObject();
    }
    if (!argsExtractor.eol())
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + "Trash was found in command arguments");
    auto res = BusinessLogic::addChatToTaget(std::move(chatToAdd));
    if (!res)
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    else
      return MsgUtils::sendResponseMessage(msg.getPeerId(), "Successfully registered this chat as target!");
  }
}
