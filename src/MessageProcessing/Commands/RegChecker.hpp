#ifndef COMMAND_REG_CHECKER_HPP
#define COMMAND_REG_CHECKER_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::commands
{
  void regChecker(const vk::objects::Message &message);
}
#endif
