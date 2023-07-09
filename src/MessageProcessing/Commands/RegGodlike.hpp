#ifndef COMMAND_REG_GODLIKE_HPP
#define COMMAND_REG_GODLIKE_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void regGodlike(const vk::objects::Message &message, size_t pos);
}
#endif
