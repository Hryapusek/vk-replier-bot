#include "TagsPool.hpp"
#include "Tags/Tags.hpp"

namespace msg_proc::tags
{
  const std::vector< std::shared_ptr< TagI > > TagsPool::vec = {
    std::make_shared< AllTag >(std::vector<std::string>{"_всем_"}),
    std::make_shared< AllImportantTag >(std::vector<std::string>{"_всем_важно_"})
  };
}
