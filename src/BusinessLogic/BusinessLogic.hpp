#ifndef BUSINESS_LOGIC_HPP
#define BUSINESS_LOGIC_HPP

#include <string>
#include <optional>
#include "../GeneralUtils/Result.hpp"
#include "../VkApi/GeneralTypes.hpp"
#include "ConfigTypes/Chats.hpp"

class BusinessLogic
{
private:
  using SourceChat_t = config::types::SourceChat_t;
  using TargetChat_t = config::types::TargetChat_t;

public:
  using VkUserId_t = config::types::VkUserId_t;
  using VkChatId_t = config::types::VkChatId_t;
  using ChatId_t = config::types::ChatId_t;
  struct Chat_t
  {
    std::optional<ChatId_t> chatId;
    VkChatId_t vkChatId;
    std::string title;
    SourceChat_t toSourceChatT();
    TargetChat_t toTargetChatT();
  };
  static void init();
  static Result<std::string> getTagAllString(VkChatId_t callerChatId);
  static Result<void> addChatToTaget(Chat_t chatToAdd);

private:
  static const std::string configPath;
  static const std::string logFilePath;
};

#endif
