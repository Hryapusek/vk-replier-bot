#ifndef NEWMESSAGE_HPP
#define NEWMESSAGE_HPP

#include "EventObject.hpp"

#include <string>
#include <jsoncpp/json/json.h>
#include "../../Objects/Message.hpp"

/**
 * @brief Class for message_new event type. It contains only
 * one Message object.
 */
class NewMessage : public EventObject
{
public:
  Message &getMessage() { return message; }
  const Message &getMessage() const { return message; }
  /// @throw Json::Exception
  static NewMessage fromJson(std::reference_wrapper<Json::Value>);

private:
  Message message;
  void foo() override {}
};

#endif
