#include "TagAll.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace message_processing::tags
{
  using namespace utils;
  void tagAll(const Message &message, size_t pos)
  {
    static const std::string tagName = "TAG:ALL";
    if (!checkMode(config::Mode::WORK, tagName, "Skipping message")
        || !checkIfSourceChatPresent(tagName, "Source chat not registered. Skipping message")
        || !checkIfChatIsSource(message.getPeerId(), tagName, "Tag used outside the source chat. Skipping message"))
      return;
    std::string title;
    try
    {
      BOOST_LOG_TRIVIAL(info) << "Extracting title";
      title = extractTitle(message.getText(), pos);
    }
    catch (const std::exception &e)
    {
      MessagesSendRequest()
      .peer_id(message.getPeerId())
      .random_id(0)
      .message("Bad title found. After tag should be a new line or a quoted sentence. Make sure that quotes are closed")
      .execute();
      return;
    }
    BOOST_LOG_TRIVIAL(info) << "Forwarding messages";
    sendMessageToAllTargets(std::move(title), message.getId());
  }
}
