#include "StatusCommand.hpp"
#include "../NewUtils.hpp"

namespace msg_proc::commands
{
  void StatusCommand::execute(const Message_t &msg)
  {
    static const std::string commandName = "status";
    static const std::string text = 
    "Bot made by Абраамян Александр, студент программной инженерии, 3 курс.\n\
    Если есть какие-то вопросы, писать сюда - vk.com/ezhiklisiy";
    utils::sendResponseMessage(msg.getPeerId(), text);
    return;
  }
}

