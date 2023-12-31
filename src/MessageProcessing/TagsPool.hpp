#ifndef TAGS_POOL_HPP
#define TAGS_POOL_HPP

#include <memory>
#include "Interfaces/TagI.hpp"

namespace msg_proc::tags
{
  class TagsPool
  {
  public:
    static const std::vector< std::shared_ptr< TagI > > tags;
  };
}

#endif
