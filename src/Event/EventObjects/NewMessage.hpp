#ifndef NEWMESSAGE_HPP
#define NEWMESSAGE_HPP

#include "EventObject.hpp"

#include <string>
#include "../../Objects/Message.hpp"

/**
 * @brief Class for message_new event type. It contains only
 * one Message object.
 */
class NewMessage : public EventObject
{

private:
  Message message;
  void foo() override {}
};

#endif
