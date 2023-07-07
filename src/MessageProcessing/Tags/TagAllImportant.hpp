#ifndef TAG_ALL_IMPORTANT_HPP
#define TAG_ALL_IMPORTANT_HPP

#include "../../VkApi/Objects/Message.hpp"

namespace message_processing::tags
{
  void tagAllImportant(const vk::objects::Message &message, size_t pos);
}
#endif
