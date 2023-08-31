#ifndef BUSINESS_LOGIC_HPP
#define BUSINESS_LOGIC_HPP

#include <string>
#include "../GeneralUtils/Result.hpp"
#include "../VkApi/GeneralTypes.hpp"

class BusinessLogic
{
  using UserId_t = vk::UserId_t;
  using ChatId_t = vk::ChatId_t;
public:
  static void init();
  static Result<std::string> getTagAllString(UserId_t callerId, ChatId_t callerChatId);

private:
  static const std::string configPath;
  static const std::string logFilePath;
};

#endif
