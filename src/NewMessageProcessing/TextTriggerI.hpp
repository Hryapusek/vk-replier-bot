#ifndef TEXT_TRIGGERI_HPP
#define TEXT_TRIGGERI_HPP

#include "../VkApi/Objects/Message.hpp"

class TextTriggerI
{
protected:
  using Message_t = vk::objects::Message;

public:
  virtual void execute(const Message_t &msg) = 0;
  virtual bool isThisTextTrigger(const Message_t &msg) = 0;
};

#endif
