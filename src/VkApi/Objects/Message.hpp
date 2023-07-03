#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <jsoncpp/json/json.h>

namespace vk::objects
{
  class Message
  {
    static const int CHAT_START_ID = 2e9;
  public:
    int id;
    /// @brief Can be user, chat or group. Destination peer_id.
    /// @note peer_id value is from bot perspective.
    /// Same conversation has different peer_id respectively to users.
    int peer_id;
    /// @brief Can be user or group. But surely not chat.
    int from_id;
    std::string text;
    bool fromDirect() const;
    bool fromChat() const;
    bool fromGroup() const;
    /// @throw Json::Exception
    static Message fromJson(std::reference_wrapper< Json::Value >);
  };
}

#endif
