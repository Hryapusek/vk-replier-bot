#ifndef COMMAND_STATUS_HPP
#define COMMAND_STATUS_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void status(const vk::objects::Message &message);
}
#endif
