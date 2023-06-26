#include "Message.hpp"

bool Message::fromDirect() const
{
  return peer_id > 0 && peer_id < CHAT_START_ID;
}

bool Message::fromChat() const
{
  return peer_id > CHAT_START_ID;
}

bool Message::fromGroup() const
{
  return peer_id < 0;
}


