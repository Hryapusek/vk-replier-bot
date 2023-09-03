#ifndef BUSINESS_LOGIC_HPP
#define BUSINESS_LOGIC_HPP

#include <string>
#include <optional>
#include "GeneralUtils/Result.hpp"
#include "VkApi/GeneralTypes.hpp"
#include "ConfigTypes/SimpleTypes.hpp"

class BusinessLogic
{
private:
  using SourceChat_t = config::types::SourceChat_t;
  using TargetChat_t = config::types::TargetChat_t;

public:
  using VkUserId_t = config::types::VkUserId_t;
  using VkChatId_t = config::types::VkChatId_t;
  using ChatId_t = config::types::ChatId_t;
  struct Chat_t // TODO separate target and source chat types
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
  static Result<void> addChatToSource(Chat_t chatToAdd);
  static Result<void> addGodlike(VkUserId_t callerId, VkUserId_t newGodlike);
  static Result<void> removeTargetChatById(VkUserId_t callerId, ChatId_t chatId);
  static Result<void> removeTargetChatByVkChatId(VkChatId_t chatId);
  static Result<void> removeSourceChat(VkUserId_t callerId);
  static Result<std::string> changeMode(VkUserId_t callerId);
  /// @note Direct access to config field. NOT THREAD-SAFE.
  static int getPort();
  static std::string getSecretString();

private:
  static const std::string configPath;
  static const std::string logFilePath;
};

#endif
