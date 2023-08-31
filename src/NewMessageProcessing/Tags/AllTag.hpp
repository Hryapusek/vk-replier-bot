#ifndef TAG_ALL_HPP
#define TAG_ALL_HPP

#include "../../VkApi/Objects/Message.hpp"
#include "../TagI.hpp"

namespace tags
{
  class AllTag : public TagI
  {
  public:
    AllTag();
    virtual void execute(const Message_t &msg);

  private:
    static const std::vector<std::string> triggers_;
  };
}
#endif
