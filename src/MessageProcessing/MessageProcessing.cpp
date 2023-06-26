#include "MessageProcessing.hpp"
#include "../../ConfigReader.hpp"
#include "../../ApiRequests/ApiRequests.hpp"

namespace
{
  using namespace commands;

  /**
   * @param[in] text message contains command.
   * @param[in] pos start position of the command.
   * @param[out] num number extracted from command.
   * Value is not changed if no number was found.
   * @throws \b std::out_of_range if number is incorrect.
   * @throws \b std::invalid_argument if number is incorrect.
   * @return True if number present in command.
   * @return False if no number found.
   */
  bool hasNumInCommand(const std::string &text, size_t pos, int &num)
  {
    std::string::const_iterator beg(&text[pos]);
    while (beg != text.cend() && !isspace(*beg))
      ++beg;
    while (beg != text.cend() && (*beg == ' ' || *beg == '\t'))
      ++beg;
    if (beg == text.cend() || *beg == '\n')
      return false;
    // auto end = std::find_if(beg, text.cend(), isspace);
    size_t argPos = std::distance(text.begin(), beg);
    auto tNum = std::stoi(text, &argPos);
    num = tNum;
    return true;
  }

  void regTarget(const Message &message, size_t pos)
  {
    if (!message.fromChat())
    {
      //TODO log that command not from chat.
      return;
    }
    auto config = config::ConfigHolder::getReadOnlyConfig().config;
    if (config.sourceChatId && config.sourceChatId.value() == message.peer_id)
    {
      //TODO error message that chat is source.
      return;
    }
    if (config.targetsTable.value().containsTarget(Target{ message.peer_id }))
    {
      //TODO error message that target already present somewhere in table.
      return;
    }
    //TODO extract num from message.text
    int num;
    try
    {
      auto registered_peer_id = config.targetsTable.value().at(num);
      if (message.peer_id == registered_peer_id)
      {
        //TODO message that this target already registered with given num.
      }
      else
      {
        //TODO error message that num is already busy.
        return;
      }
    }
    catch (const std::exception &e)
    { }
  }

  void sendMessageToAllTargets(std::string &&title, int forward_message_id)
  {
    cpr::Parameters params {
      { "peer_ids", config::ConfigHolder::getTargetIds() },
      { "random_id", "0" },
      { "forward_messages", std::to_string(forward_message_id) },
      { "message", std::move(title) }
    };
    vk::sendRequest("messages.send", std::move(params));
  }

  std::string getTitle(const std::string &text, size_t pos)
  {
    auto begin = text.find_first_of(" \n", pos);
    if (begin == std::string::npos || text[begin] == '\n')
      return "";
    begin = text.find_first_not_of(" \n", begin);
    auto end = text.find('\n', begin);
    return text.substr(begin, end - begin);
  }

  Tag findTag(const std::string &text, size_t &pos)
  {
    for (const auto & [tag, strings_to_find] : tagStrings)
      for (const auto &str : strings_to_find)
        if (text.find(str) != std::string::npos)
          return tag;
    return Tag::NONE;
  }

  Command findCommand(const std::string &text, size_t &pos)
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
    using namespace config;
    const Message &message = newMessage->getMessage();
    Tag tag;
    Command cmd;
    size_t pos = 0;
    Mode mode = ConfigHolder::getReadOnlyConfig().config.mode;
    if (mode == Mode::WORK)
    {
      tag = findTag(message.text, pos);
      switch (tag)
      {
      case Tag::ALL:
      {
        std::string title = getTitle(message.text, pos);
        sendMessageToAllTargets(std::move(title), message.id);
        break;
      }

      case Tag::ALL_IMPORTANT:
      {
        std::string title = "@all, " + getTitle(message.text, pos);
        sendMessageToAllTargets(std::move(title), message.id);
        break;
      }

      case Tag::NONE:

        break;

      default:
        //holy crap cant even imagine how you get there
        break;
      }
    }
    else if (mode == Mode::CONFIG)
    {
      cmd = findCommand(message.text, pos);
      switch (cmd)
      {
      case Command::REG_TARGET:
      {
        regTarget(message, pos);
        break;
      }
      default:
        break;
      }
    }
  }
}
