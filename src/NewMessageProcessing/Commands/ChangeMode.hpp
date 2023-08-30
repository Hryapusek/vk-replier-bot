#ifndef COMMAND_CHANGE_MODE_HPP
#define COMMAND_CHANGE_MODE_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void changeMode(const vk::objects::Message &message);
}
#endif
