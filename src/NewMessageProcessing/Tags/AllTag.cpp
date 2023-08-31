#include "AllTag.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../Business logic/BusinessLogic.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace tags
{
  using namespace utils;
  void tagAll(const Message &message, size_t pos)
  {
    static const std::string tagName = "TAG:ALL";
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfSourceChatPresent(req, tagName, "Source chat not registered. Skipping message")
        || !checkIfChatIsSource(message.getPeerId(), tagName, "Tag used outside the source chat. Skipping message")
        || !checkMode(config::Mode::WORK, req, tagName, "Skipping message"))
      return;
    std::string title;
    try
    {
      BOOST_LOG_TRIVIAL(info) << "Extracting title";
      title = extractTitle(message.getText(), pos);
    }
    catch (const std::exception &e)
    {
      title = "";
    }
    BOOST_LOG_TRIVIAL(info) << "Forwarding messages";
    sendMessageToAllTargets(std::move(title), message.getConversationMessageId());
    req.message("Successfully forwarded!").execute();
  }


  void tags::AllTag::execute(const Message_t &msg)
  { 
    auto res = BusinessLogic::getTagAllString(msg.getFromId(), msg.getPeerId());
    if (!res)
    {
      // TODO send error message
    }
    auto targetsStr = std::move(res.getObject());
  }
}
