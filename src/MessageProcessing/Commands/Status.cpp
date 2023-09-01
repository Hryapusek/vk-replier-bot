#include "Status.hpp"
#include <boost/log/trivial.hpp>
#include "../NewUtils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace message_processing::commands
{
  using namespace utils;
  void status(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "status";
    static str_cref text = "Bot made by Абраамян Александр, студент программной инженерии, 3 курс.\n\
                            Если есть какие-то вопросы, писать сюда - vk.com/ezhiklisiy";
    MessagesSendRequest()
    .random_id(0)
    .message("Bot made by Абраамян Александр, студент программной инженерии, 3 курс.\nvk.com/ezhiklisiy")
    .peer_id(message.getPeerId())
    .execute();
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully sent status to " << std::to_string(message.getFromId());
    return;
  }
}

namespace msg_proc::commands
{
  void StatusCommand::execute(const Message_t &msg)
  {
    static const std::string commandName = "status";
    static const std::string text = "Bot made by Абраамян Александр, студент программной инженерии, 3 курс.\n\
                            Если есть какие-то вопросы, писать сюда - vk.com/ezhiklisiy";
    utils::
  }
}

