#include "AllImportantTag.hpp"
#include "../ArgsExtractor.hpp"
#include "../NewUtils.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"
#include <string>


namespace
{
  void sendErrorResponseMessage(vk::Peer_Id_t destPeerId, str_cref errorText)
  {
    using namespace std::literals;
    msg_utils::sendResponseMessage(destPeerId, "Error while forwarding messages. "s + errorText);
  }
}

namespace tags
{
  const std::vector<std::string> AllImportantTag::triggers_ = {"_всем_важно_"};

  AllImportantTag::AllImportantTag() : TagI(triggers_) {}

  // TODO add logging
  void AllImportantTag::execute(const Message_t &msg)
  {
    using namespace std::literals;
    auto res = BusinessLogic::getTagAllString(msg.getFromId(), msg.getPeerId());
    if (!res)
    {
      sendErrorResponseMessage(msg.getPeerId(), res.getErrorMessage());
      return;
    }
    auto targetsStr = std::move(res.getObject());
    ArgsExtractor argsExtractor(findTriggerBegin(msg.getText(), triggers_), msg.getText().end(), true);
    std::string title = "";
    if (argsExtractor.hasQuotedString())
    {
      auto res = argsExtractor.extractQuotedString();
      if (!res)
      {
        sendErrorResponseMessage(msg.getPeerId(), res.getErrorMessage());
        return;
      }
      title = std::move(res.getObject());
    }
    msg_utils::sendMessageToAllTargets("@all, "s + title, msg.getConversationMessageId());
    msg_utils::sendResponseMessage(msg.getPeerId(), "Successfully forwarded!"s);
  }
}
