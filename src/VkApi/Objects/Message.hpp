#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <optional>
#include <jsoncpp/json/json.h>

namespace vk::objects
{
  class Message
  {
  public:
    static const int CHAT_START_ID;
    std::optional< int > id;
    /// @brief Can be user, chat or group. Destination peer_id.
    /// @note peer_id value is from bot perspective.
    /// Same conversation has different peer_id respectively to users.
    std::optional< int > peer_id;
    /// @brief Can be user or group. But surely not chat.
    std::optional< int > from_id;
    std::optional< std::string > text;
    bool fromDirect() const;
    bool fromChat() const;
    bool fromGroup() const;
    /// @throw Json::Exception
    static Message fromJson(const Json::Value &);
  };
}

#endif
