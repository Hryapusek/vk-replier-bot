#include "AllTag.hpp"
#include "../ArgsExtractor.hpp"
#include "../NewUtils.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"
#include <string>

namespace msg_proc::tags
{
  // TODO add logging
  void AllTag::execute(const Message_t &msg)
  {
    using namespace std::literals;
    static const std::string commandErrorText = "Error while forwarding messages. "s;
    auto res = BusinessLogic::getTagAllString(msg.getPeerId());
    if (!res)
      return MsgUtils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
    auto targetsStr = std::move(res.getObject());
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText()), msg.getText().end(), true);
    std::string title = "";
    if (argsExtractor.hasQuotedString())
    {
      auto res = argsExtractor.extractQuotedString();
      if (!res)
        return MsgUtils::sendResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      title = std::move(res.getObject());
    }
    MsgUtils::sendMessageToAllTargets(title, msg.getConversationMessageId(), targetsStr, msg.getPeerId());
    MsgUtils::sendResponseMessage(msg.getPeerId(), "Successfully forwarded!");
  }
}
