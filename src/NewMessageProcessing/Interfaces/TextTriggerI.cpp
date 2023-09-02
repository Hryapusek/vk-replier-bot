#include "TextTriggerI.hpp"
#include "../NewUtils.hpp"

namespace msg_proc
{
  TextTriggerI::TextTriggerI(WordsContainer_t triggerWords) :
    triggerWords_(std::move(triggerWords))
  {  }

  bool TextTriggerI::isThisTrigger(const Message_t &msg)
  {
    for (const auto &wordToFind : triggerWords_)
      if (MsgUtils::findWord(msg.getText(), wordToFind) != msg.getText().cend())
        return true;
    return false;
  }

  std::string::const_iterator TextTriggerI::findTriggerBegin(const std::string &text)
  {
    for (const auto &wordToFind : triggerWords_)
    {
      auto it = MsgUtils::findWord(text, wordToFind);
      if (it != text.cend())
        return it;
    }
    return text.cend();
  }
}
