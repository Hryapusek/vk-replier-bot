#ifndef COMMAND_DEL_TARGET_BY_NUM_HPP
#define COMMAND_DEL_TARGET_BY_NUM_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void delTargetByNum(const vk::objects::Message &message, size_t pos);
}
#endif
