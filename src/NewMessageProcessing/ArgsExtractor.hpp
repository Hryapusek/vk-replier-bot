#ifndef ARGS_EXTRACTOR_HPP
#define ARGS_EXTRACTOR_HPP

#include <string>
#include "../GeneralUtils/Result.hpp"

class ArgsExtractor
{
public:
  using Iterator_t = std::string::const_iterator;
  ArgsExtractor(Iterator_t begin, Iterator_t end, bool skipFirstWord);
  Result<std::string> extractQuotedString();
  bool hasQuotedString();
  Result<int> extractInt();
  bool hasInt();
  bool eol();
};

#endif
