#include "AllImportantTag.hpp"
#include "MessageProcessing/MsgUtils.hpp"
#include "MessageProcessing/ArgsExtractor.hpp"
#include "BusinessLogic/BusinessLogic.hpp"

namespace msg_proc::tags
{
  void AllImportantTag::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Error while forwarding messages. "s;
    static const std::string textTriggerName = "AllImportantTag"s;
    MsgUtils::logTextTriggerName(textTriggerName);
    auto res = BusinessLogic::getTagAllString(msg.getPeerId());
    if (!res)
      return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    auto targetsStr = std::move(res.getObject());
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    std::string title = "@all";
    if (argsExtractor.hasQuotedString())
    {
      auto res = argsExtractor.extractQuotedString();
      if (!res)
        return MsgUtils::logAndSendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      title += ", " + std::move(res.getObject());
    }
    MsgUtils::sendMessageToAllTargets(title, msg.getConversationMessageId(), targetsStr, msg.getPeerId());
    MsgUtils::logAndSendResponseMessage(msg.getPeerId(), "Successfully forwarded!"s);
  }
}
