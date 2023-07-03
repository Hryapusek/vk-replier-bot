#ifndef CHATS_HPP
#define CHATS_HPP

#include <string>
#include <optional>

struct TargetChat
{
  int num;
  int peer_id;
  std::optional<std::string> title;
};

struct SourceChat
{
  int peer_id;
  std::optional<std::string> title;
};


#endif
