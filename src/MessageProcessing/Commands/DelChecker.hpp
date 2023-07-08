#ifndef COMMAND_DEL_CHECKER_HPP
#define COMMAND_DEL_CHECKER_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void delChecker(const vk::objects::Message &message);
}
#endif
