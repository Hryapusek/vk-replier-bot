#ifndef TAG_ALL_IMPORTANT_HPP
#define TAG_ALL_IMPORTANT_HPP

#include "../../VkApi/Objects/Message.hpp"
#include "../TagI.hpp"

namespace tags
{
  class AllImportantTag : public TagI
  {
  public:
    AllImportantTag();
    virtual void execute(const Message_t &msg);

  private:  
    static const std::vector<std::string> triggers_;
  };
}

#endif
