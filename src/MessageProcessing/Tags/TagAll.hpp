#ifndef TAG_ALL_HPP
#define TAG_ALL_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::tags
{
  void tagAll(const vk::objects::Message &message, size_t pos);
}
#endif
