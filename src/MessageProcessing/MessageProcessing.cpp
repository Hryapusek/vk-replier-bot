#include "MessageProcessing.hpp"

#include <boost/log/trivial.hpp>
#include <regex>

#include "Commands.hpp"
#include "Tags.hpp"
#include "../ConfigReader.hpp"
#include "../VkApi/Requests/Messages/MessagesSendRequest.hpp"
#include "../VkApi/Exceptions.hpp"

// TODO shorten title
// TODO add /help

namespace
{
  using namespace message_processing;
  using namespace vk::requests::messages;
  using namespace vk::objects;
  using namespace vk::callback::event::objects;
  using str_cref = const std::string &;

  /// @note id, from_id, peer_id and text should be in message
  void logMessage(const vk::objects::Message &msg)
  {
    BOOST_LOG_TRIVIAL(info) << "Current message:\n"
                            << "\tid: " << msg.getId() << '\n'
                            << "\tfrom_id: " << msg.getFromId() << '\n'
                            << "\tpeer_id: " << msg.getPeerId() << '\n'
                            << "\ttext: " << msg.getText();
  }

  bool hasMessageNecessaryFields(const vk::objects::Message &m)
  {
    return m.hasFromId() && m.hasId() && m.hasPeerId() && m.hasText();
  }

  void logAndSendErrorMessage(const std::exception &e, int peer_id, str_cref exceptionName)
  {
    BOOST_LOG_TRIVIAL(error) << "Unexpected " << exceptionName << " was thrown. Handling current event aborted.\n" << e.what();
    try
    {
      MessagesSendRequest req;
      req.random_id(0).peer_id(peer_id).message(e.what()).execute();
    } catch (const std::exception &e)
    {
      BOOST_LOG_TRIVIAL(error) << "Error while sending exception message";
    }
  }

  Tag findTag(str_cref text, size_t &pos)
  {
    for (const auto & [tag, strings_to_find] : tagStrings)
      for (const auto &str : strings_to_find)
      {
        std::string regexText = "(^|[ ]+)" + str + "($|[ ]+)";
        std::regex regex(regexText);
        std::smatch res;
        bool isFound = std::regex_search(text, res, regex);
        if (isFound)
        {
          pos = std::distance(text.cbegin(), res[0].first);
          return tag;
        }
      }
    return Tag::NONE;
  }

  std::string extractWord(std::string::const_iterator cbeg, str_cref text)
  {
    if (cbeg == text.cend())
      return "";
    auto wordBeg = cbeg;
    auto wordEnd = cbeg;
    while (wordEnd != text.cend() && !isspace(*wordEnd))
      ++wordEnd;
    return text.substr(std::distance(text.cbegin(), wordBeg), std::distance(wordBeg, wordEnd));
  }

  Command findCommand(str_cref text, size_t &pos)
  {
    auto cmdBeg = std::find(text.cbegin(), text.cend(), '/');
    if (cmdBeg == text.cend())
      return Command::NONE;
    pos = std::distance(text.cbegin(), cmdBeg);
    std::string command = extractWord(++cmdBeg, text);
    for (const auto & [cmd, strings_to_find] : commandStrings)
      for (const auto &str : strings_to_find)
        if (command == str)
          return cmd;
    return Command::NONE;
  }
}

namespace message_processing
{
  void processMessageWithCommand(const Message &message, Command cmd, size_t pos);

  void processMessageWithTag(const Message &message, Tag tag, size_t pos);

  void processMessage(std::shared_ptr< NewMessage > newMessage)
  {
    using namespace config;
    if (!hasMessageNecessaryFields(newMessage->getMessage()))
    {
      BOOST_LOG_TRIVIAL(info) << "Message does not contain necessary fields. Skipping";
      return;
    }
    if (newMessage->getMessage().getFromId() == config::ConfigHolder::getGroupId())
    {
      BOOST_LOG_TRIVIAL(info) << "Message from bot. Skipping";
      return;
    }
    logMessage(newMessage->getMessage());
    try
    {
      size_t pos;
      Tag tag = findTag(newMessage->getMessage().getText(), pos);
      if (tag != Tag::NONE)
      {
        BOOST_LOG_TRIVIAL(info) << "Message with tag";
        processMessageWithTag(newMessage->getMessage(), tag, pos);
        BOOST_LOG_TRIVIAL(info) << "Message tag processing finished successfully";
        return;
      }
      Command cmd = findCommand(newMessage->getMessage().getText(), pos);
      if (cmd != Command::NONE)
      {
        BOOST_LOG_TRIVIAL(info) << "Message with command";
        processMessageWithCommand(newMessage->getMessage(), cmd, pos);
        BOOST_LOG_TRIVIAL(info) << "Message command processing finished successfully";
        return;
      }
      BOOST_LOG_TRIVIAL(info) << "Neither a command nor a tag were found. Skipping";
    }
    catch (const Json::Exception &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "Json exception");
    }
    catch (const std::logic_error &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "std::logic_error");
    }
    catch (const vk::exceptions::RequestException &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "RequestException");
    }
    catch (const std::exception &e)
    {
      logAndSendErrorMessage(e, newMessage->getMessage().getPeerId(), "std::excepiton");
    }
  }

  void processMessageWithCommand(const Message &message, Command cmd, size_t pos)
  {
    using namespace commands;
    switch (cmd)
    {
      case Command::REG_TARGET:
      {
        BOOST_LOG_TRIVIAL(info) << "REG_TARGET command found";
        regTarget(message, pos);
        break;
      }
      case Command::REG_SOURCE:
      {
        BOOST_LOG_TRIVIAL(info) << "REG_SOURCE command found";
        regSource(message, pos);
        break;
      }
      case Command::DEL_TARGET:
      {
        BOOST_LOG_TRIVIAL(info) << "DEL_TARGET command found";
        delTarget(message);
        break;
      }
      case Command::DEL_SOURCE:
      {
        BOOST_LOG_TRIVIAL(info) << "DEL_SOURCE command found";
        delSource(message);
        break;
      }
      case Command::REG_GODLIKE:
      {
        BOOST_LOG_TRIVIAL(info) << "REG_GODLIKE command found";
        regGodlike(message, pos);
        break;
      }
      case Command::CHANGE_MODE:
      {
        BOOST_LOG_TRIVIAL(info) << "CHANGE_MODE command found";
        changeMode(message);
        break;
      }
      case Command::DEL_TARGET_BY_NUM:
      {
        BOOST_LOG_TRIVIAL(info) << "DEL_TARGET_BY_NUM command found";
        delTargetByNum(message, pos);
        break;
      }
      case Command::STATUS:
      {
        BOOST_LOG_TRIVIAL(info) << "STATUS command found";
        status(message);
        break;
      }
      case Command::NONE:
      {
        BOOST_LOG_TRIVIAL(warning) << "No command found. Skipping";
        break;
      }
      default:
        BOOST_LOG_TRIVIAL(warning) << "Command variable was not initialized. Skipping";
        break;
    }
  }

  void processMessageWithTag(const Message &message, Tag tag, size_t pos)
  {
    using namespace tags;
    switch (tag)
    {
      case Tag::ALL:
      {
        tagAll(message, pos);
        break;
      }

      case Tag::ALL_IMPORTANT:
      {
        tagAllImportant(message, pos);
        break;
      }

      case Tag::NONE:
      {
        BOOST_LOG_TRIVIAL(warning) << "TAG::NONE in processMessageWithTag. Skipping";
        break;
      }

      default:
      {
        BOOST_LOG_TRIVIAL(warning) << "Tag variable was not initialized in processMessageWithTag. Skipping";
        break;
      }
    }
  }

}
