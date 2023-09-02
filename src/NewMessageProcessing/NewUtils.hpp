// TODO add header guards
// TODO add realization
#include <string>
#include "../VkApi/GeneralTypes.hpp"
#include "../VkApi/Objects/Message.hpp"

namespace
{
  using Iterator_t = std::string::iterator;
  using str_cref = const std::string &;
  using Message_t = vk::objects::Message;
}

namespace msg_proc::utils
{
  Iterator_t findWord(Iterator_t begin, Iterator_t end, std::string wordToFind);
  void sendSimpleMessage(vk::types::Peer_Id_t destPeerId, str_cref text);
  void sendMessageToAllTargets(str_cref text, int fwd_conv_msg_id);
  void sendResponseMessage(vk::types::Peer_Id_t destPeerId, str_cref text);
  void sendErrorResponseMessage(vk::types::Peer_Id_t destPeerId, str_cref errorText);
}
