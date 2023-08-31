#ifndef TEXT_TRIGGERI_HPP
#define TEXT_TRIGGERI_HPP

#include <vector>
#include "../VkApi/Objects/Message.hpp"

class TextTriggerI
{
protected:
  using Message_t = vk::objects::Message;
  using WordsContainer_t = std::vector<std::string>;

public:
  TextTriggerI(WordsContainer_t triggerWords);
  virtual void execute(const Message_t &msg) = 0;
  virtual bool isThisTextTrigger(const Message_t &msg);
  std::string::iterator findTriggerBegin(const std::string &text, const std::vector<std::string> wordsToFind);

protected:
  WordsContainer_t triggerWords_;

};

#endif
