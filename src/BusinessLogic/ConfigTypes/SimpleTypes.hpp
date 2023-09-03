#ifndef SIMPLE_TYPES_HPP
#define SIMPLE_TYPES_HPP

#include <string>
#include <optional>
#include "VkApi/GeneralTypes.hpp"

namespace config::types
{
  using ChatId_t = int;
  using VkUserId_t = vk::types::UserId_t;
  using VkChatId_t = vk::types::ChatId_t;

  struct TargetChat_t
  {
    std::optional< ChatId_t > chatId;
    VkChatId_t vkChatId;
    std::string title;
  };

  struct SourceChat_t
  {
    VkChatId_t vkChatId;
    std::string title;
  };

  enum Mode
  {
    WORK,
    CONFIG
  };
}

#endif
