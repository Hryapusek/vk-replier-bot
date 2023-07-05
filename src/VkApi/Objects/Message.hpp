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
    
    bool hasId() const { return id.has_value(); }
    bool hasPeerId() const { return peer_id.has_value(); }
    bool hasFromId() const { return from_id.has_value(); }
    bool hasText() const { return text.has_value(); }

    /// @throw std::bad_optional_access
    int getId() const { return *id; }

    /// @brief Can be user, chat or group. Destination peer_id.
    /// @throw std::bad_optional_access
    /// @note peer_id value is from bot perspective.
    /// Same conversation has different peer_id respectively to users.
    int getPeerId() const { return *peer_id; }

    /// @brief Can be user or group. But surely not chat.
    /// @throw std::bad_optional_access
    int getFromId() const { return *from_id; }

    /// @throw std::bad_optional_access
    const std::string &getText() const { return *text; }

    /// @throw std::bad_optional_access
    bool fromDirect() const;

    /// @throw std::bad_optional_access
    bool fromChat() const;

    /// @throw std::bad_optional_access
    bool fromGroup() const;

    /// @throw Json::Exception
    static Message fromJson(const Json::Value &);

  private:
    std::optional< int > id;
    std::optional< int > peer_id;
    std::optional< int > from_id;
    std::optional< std::string > text;
  };
}

#endif
