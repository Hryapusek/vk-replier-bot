// TODO add header guards
#include <string>

namespace
{
  using Iterator_t = std::string::iterator;
}

Iterator_t findWord(Iterator_t begin, Iterator_t end, std::string wordToFind);