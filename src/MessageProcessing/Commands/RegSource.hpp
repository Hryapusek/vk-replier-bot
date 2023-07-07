#ifndef COMMAND_REG_SOURCE_HPP
#define COMMAND_REG_SOURCE_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void regSource(const vk::objects::Message &message, size_t pos);
}
#endif
