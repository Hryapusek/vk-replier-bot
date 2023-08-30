#ifndef COMMAND_DEL_TARGET_HPP
#define COMMAND_DEL_TARGET_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void delTarget(const vk::objects::Message &message);
}
#endif
