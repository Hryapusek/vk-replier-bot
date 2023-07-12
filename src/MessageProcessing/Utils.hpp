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

  /// @brief Extracts words until quote.
  /// @throw \b std::exception if string is not closed with -> "
  /// @return extracted quoted string
  std::string extractString(std::string::const_iterator &beg, str_cref text);

  /// @return extracted num
  /// @throw std::logic_error if num not found or number is bad.
  int extractNum(str_cref text, size_t pos);

  /// @throws \b std::out_of_range, if number is incorrect.
  /// @throws \b std::invalid_argument if number is incorrect.
  /// @throws \b std::exception if after number are unquoted words on the same line.
  void extractNumAndTitle(str_cref text, size_t pos, std::optional< int > &num, std::string &title);

  /// @return Empty string if no quotes were found right after command.
  /// Quoted title otherwise.
  /// @throw \b std::exception if string is not closed with -> "
  std::string extractTitle(str_cref text, size_t pos);
  
  void logAndSendErrorMessage(vk::requests::messages::MessagesSendRequest &req, str_cref commandName, str_cref errorMessage);

  void sendMessageToAllTargets(str_cref text, int fwd_conv_msg_id);

  bool checkIfCommandFromChat(const vk::objects::Message &message, str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsNotSource(int peerId, vk::requests::messages::MessagesSendRequest &req,
                              str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsNotSource(int peerId, str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsSource(int peerId, str_cref commandName, str_cref errorMessage);

  bool checkIfChatIsSource(int peerId, vk::requests::messages::MessagesSendRequest &req,
                           str_cref commandName, str_cref errorMessage);

  bool checkMode(config::Mode mode, str_cref commandName, str_cref errorMessage);

  bool checkMode(config::Mode mode, vk::requests::messages::MessagesSendRequest &req,
                 str_cref commandName, str_cref errorMessage);

  bool checkIfSourceChatPresent(str_cref commandName, str_cref errorMessage);

  bool checkIfSourceChatPresent(vk::requests::messages::MessagesSendRequest &req,
                                str_cref commandName, str_cref errorMessage);

  bool checkIfPeerIdInTargetsTable(const vk::objects::Message &message, vk::requests::messages::MessagesSendRequest &req,
                                   str_cref commandName, str_cref errorMessage);

  bool checkIfPeerIdNotInTargetsTable(const vk::objects::Message &message, vk::requests::messages::MessagesSendRequest &req,
                                      str_cref commandName, str_cref errorMessage);

  bool checkIfFromDirect(const vk::objects::Message &message, str_cref commandName, str_cref errorMessage);

  bool checkIfGodlike(int peerId, str_cref commandName, str_cref errorMessage);

  bool checkIfGodlike(int peerId, vk::requests::messages::MessagesSendRequest &req,
                      str_cref commandName, str_cref errorMessage);

  bool checkIfNotGodlike(int peerId, vk::requests::messages::MessagesSendRequest &req,
                         str_cref commandName, str_cref errorMessage);

  bool checkIfCanChangeModeTo(config::Mode mode, vk::requests::messages::MessagesSendRequest &req,
                              str_cref commandName, str_cref errorMessage);

  bool checkIfNumInTargetsTable(int num, vk::requests::messages::MessagesSendRequest &req,
                                str_cref commandName, str_cref errorMessage);

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
