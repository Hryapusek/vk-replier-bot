#ifndef MESSAGE_PROCESSING_UTILS_HPP
#define MESSAGE_PROCESSING_UTILS_HPP

#include <string>
#include <optional>
#include "../VkApi/Objects/Message.hpp"
#include "../ConfigReader.hpp"
#include "../VkApi/Requests/Messages/MessagesSendRequest.hpp"

namespace message_processing::utils
{
  using str_cref = const std::string &;

  /// @return True if not EOL or EOF
  bool skipWord(std::string::const_iterator &beg, str_cref text);

  /// @throw \b std::exception if string is not closed with -> "
  std::string extractString(std::string::const_iterator &beg, str_cref text);

  /// @throws \b std::out_of_range, if number is incorrect.
  /// @throws \b std::invalid_argument if number is incorrect.
  /// @throws \b std::exception if command format is incorrect.
  void extractNumAndTitle(str_cref text, size_t pos, std::optional< int > &num, std::string &title);

  /// @return Empty string if no quotes were found right after command.
  /// Quoted title otherwise.
  /// @throw \b std::exception if unclosed quote found
  std::string extractTitle(str_cref text, size_t pos);
  void logAndSendErrorMessage(vk::requests::messages::MessagesSendRequest &req, str_cref commandName, str_cref errorMessage);

  void sendMessageToAllTargets(str_cref text, int fwd_msg_id);

  bool checkIfCommandFromChat(const vk::objects::Message &message, str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsNotSource(int peerId, vk::requests::messages::MessagesSendRequest &req,
                              str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsNotSource(int peerId, str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsSource(int peerId, str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsSource(int peerId, vk::requests::messages::MessagesSendRequest &req,
                           str_cref commandName, str_cref errorMessage);

  bool checkMode(config::Mode mode, str_cref commandName, str_cref errorMessage);

  bool checkIfSourceChatPresent(str_cref commandName, str_cref errorMessage);

  bool checkIfPeerIdInTargetsTable(const vk::objects::Message &message, vk::requests::messages::MessagesSendRequest &req,
                                   str_cref commandName, str_cref errorMessage);

  bool checkIfPeerIdNotInTargetsTable(const vk::objects::Message &message, vk::requests::messages::MessagesSendRequest &req,
                                      str_cref commandName, str_cref errorMessage);

  bool checkIfFromDirect(const vk::objects::Message &message, str_cref commandName, str_cref errorMessage);

  bool checkIfChecker(const vk::objects::Message &message, vk::requests::messages::MessagesSendRequest &req,
                             str_cref commandName, str_cref errorMessage);

  bool checkIfNotChecker(const vk::objects::Message &message, str_cref commandName, str_cref errorMessage);

  /// @brief if cond is False - logAndSendErrorMessage called.
  /// If True - nothing happens.
  /// @return cond
  bool checkIf(bool cond, vk::requests::messages::MessagesSendRequest &req, str_cref commandName, str_cref errorMessage);

  /// @brief if cond is False - errorMessage will be logged.
  /// If True - nothing happens.
  /// @return cond
  bool checkIf(bool cond, str_cref commandName, str_cref errorMessage);
}

#endif
