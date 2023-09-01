#ifndef CHATS_HPP
#define CHATS_HPP

#include <string>
#include <optional>
#include "../../VkApi/GeneralTypes.hpp"

namespace config::types
{
  using ChatId_t = int;
  using VkUserId_t = vk::types::UserId_t;
  using VkChatId_t = vk::types::ChatId_t;

  struct TargetChat_t
  {
    std::optional<ChatId_t> num;
    VkChatId_t peer_id;
    std::optional< std::string > title;
  };

  struct SourceChat_t
  {
    VkChatId_t peer_id;
    std::optional< std::string > title;
  };
}

#endif
