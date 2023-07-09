#include "Status.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace
{
  using namespace message_processing;
  using namespace message_processing::utils;
  using str_cref = const std::string &;
}

namespace message_processing::commands
{
  using namespace utils;
  void status(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "status";
    if (!checkIfChecker(message.getFromId(), commandName, "Can not perform command. Not from checker"))
      return;
    MessagesSendRequest()
    .random_id(0)
    .message("YAHAY BALYA")
    .peer_id(message.getPeerId())
    .execute();
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully sent status to " << std::to_string(message.getFromId());
    return;
  }
}
