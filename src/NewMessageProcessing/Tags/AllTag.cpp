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
    auto res = BusinessLogic::getTagAllString(msg.getFromId(), msg.getPeerId());
    if (!res)
    {
      utils::sendErrorResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
      return;
    }
    auto targetsStr = std::move(res.getObject());
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText(), triggerWords_), msg.getText().end(), true);
    std::string title = "";
    if (argsExtractor.hasQuotedString())
    {
      auto res = argsExtractor.extractQuotedString();
      if (!res)
      {
        utils::sendResponseMessage(msg.getPeerId(), commandErrorText + res.getErrorMessage());
        return;
      }
      title = std::move(res.getObject());
    }
    utils::sendMessageToAllTargets(title, msg.getConversationMessageId());
    utils::sendResponseMessage(msg.getPeerId(), "Successfully forwarded!");
  }
}
