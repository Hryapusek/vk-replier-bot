#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

class Message
{
public:
  int id;
  int date;
  int peer_id;
  int from_id;
  std::string text;
  int random_id;
};

#endif
