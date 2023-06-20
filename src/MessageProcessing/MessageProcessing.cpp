#include "MessageProcessing.hpp"

namespace
{
  using namespace commands;

  Tag hasTag(const std::string &text, size_t &pos)
  {
    for (const auto & [tag, strings_to_find] : tagStrings)
      for (const auto &str : strings_to_find)
        if (text.find(str) != std::string::npos)
          return tag;
    return Tag::NONE;
  }

  Command hasCommand(const std::string &text, size_t &pos)
  {
    for (const auto & [cmd, strings_to_find] : commandStrings)
      for (const auto &str : strings_to_find)
        if (text.find(str) != std::string::npos)
          return cmd;
    return Command::NONE;
  }
}

namespace commands
{

  void processMessage(std::shared_ptr< NewMessage > newMessage)
  {
    const Message &message = newMessage->getMessage();
    Tag tag;
    Command cmd;
    size_t pos = 0;
    if ((tag = hasTag(message.text, pos)) != Tag::NONE)
    {
      //TODO forward message
    }
    else if ((cmd = hasCommand(message.text, pos)) != Command::NONE)
    {
      //TODO command processing
    }
  }
}
