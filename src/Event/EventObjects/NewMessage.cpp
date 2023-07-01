#include "NewMessage.hpp"

NewMessage NewMessage::fromJson(std::reference_wrapper<Json::Value> root)
{
  NewMessage newMsg;
  newMsg.message = Message::fromJson(root.get()["object"]);
  return newMsg;
}