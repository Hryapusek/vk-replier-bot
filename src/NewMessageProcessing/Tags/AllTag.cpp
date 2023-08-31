#include "AllTag.hpp"
#include "../ArgsExtractor.hpp"
#include "../NewUtils.hpp"
#include "../../BusinessLogic/BusinessLogic.hpp"
#include "../../VkApi/Requests/Messages/MessagesSendRequest.hpp"
#include <string>

namespace tags
{
  AllTag::AllTag() : TagI(triggers_)
  {}

  const std::vector<std::string> AllTag::triggers_ = {"_всем_"};

  // TODO add logging
  void AllTag::execute(const Message_t &msg)
  {
    using namespace vk::requests::messages;
    using namespace std::literals;
    auto res = BusinessLogic::getTagAllString(msg.getFromId(), msg.getPeerId());
    if (!res)
    {
      msg_utils::sendResponseMessage(msg.getPeerId(), "Error while forwarding messages. "s + res.getErrorMessage());
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
        msg_utils::sendResponseMessage(msg.getPeerId(), "Error while forwarding messages. "s + res.getErrorMessage());
        return;
      }
      title = std::move(res.getObject());
    }
    msg_utils::sendMessageToAllTargets(title, msg.getConversationMessageId());
    msg_utils::sendResponseMessage(msg.getPeerId(), "Successfully forwarded!");
  }
}
