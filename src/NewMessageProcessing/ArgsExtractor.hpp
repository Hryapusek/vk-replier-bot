#ifndef ARGS_EXTRACTOR_HPP
#define ARGS_EXTRACTOR_HPP

#include <string>
#include "../General utils/Result.hpp"

class ArgsExtractor
{
public:
  using Iterator_t = std::string::iterator;
  ArgsExtractor(Iterator_t begin, Iterator_t end, bool skipFirstWord);
  Result<std::string> extractQuotedString();
};

#endif
