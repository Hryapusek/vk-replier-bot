#ifndef COMMANDI_HPP
#define COMMANDI_HPP

#include "TextTriggerI.hpp"

namespace msg_proc
{
  class CommandI: public TextTriggerI
  { 
    using TextTriggerI::TextTriggerI;
  };
}

#endif
