#ifndef COMMAND_REG_TARGET_HPP
#define COMMAND_REG_TARGET_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void regTarget(const vk::objects::Message &message, size_t pos);
}
#endif
