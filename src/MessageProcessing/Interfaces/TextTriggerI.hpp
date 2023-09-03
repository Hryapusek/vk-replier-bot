#ifndef TEXT_TRIGGERI_HPP
#define TEXT_TRIGGERI_HPP

#include <vector>
#include "VkApi/Objects/Message.hpp"

namespace msg_proc
{
  class TextTriggerI
  {
  protected:
    using Message_t = vk::objects::Message;
    using Words_t = std::string;
    using WordsContainer_t = std::vector< Words_t >;

  public:
    TextTriggerI(WordsContainer_t triggerWords);
    /// @note if isThisTextTrigger not true - undefined behaviour
    virtual void execute(const Message_t &msg) = 0;
    virtual bool isThisTrigger(const Message_t &msg);
  
  protected:
    Words_t::const_iterator findTriggerBegin(const std::string &text);

  protected:
    WordsContainer_t triggerWords_;
  };
}

#endif
