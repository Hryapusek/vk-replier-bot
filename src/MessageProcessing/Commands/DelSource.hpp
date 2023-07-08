#ifndef COMMAND_DEL_SOURCE_HPP
#define COMMAND_DEL_SOURCE_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void delSource(const vk::objects::Message &message);
}
#endif
