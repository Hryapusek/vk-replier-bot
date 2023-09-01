#include "TextTriggerI.hpp"

TextTriggerI::TextTriggerI(WordsContainer_t triggerWords) :
  triggerWords_(std::move(triggerWords))
{  }

bool TextTriggerI::isThisTextTrigger(const Message_t & msg)
{
  //TODO make realization
return false;
}

std::string::iterator TextTriggerI::findTriggerBegin(const std::string & text, const std::vector<std::string> wordsToFind)
{
  //TODO make realization
return std::string::iterator();
}
