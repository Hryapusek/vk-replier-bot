#ifndef ARGS_EXTRACTOR_HPP
#define ARGS_EXTRACTOR_HPP

#include <string>
#include "GeneralUtils/Result.hpp"

class ArgsExtractor
{
public:
  using CIterator_t = std::string::const_iterator;
  ArgsExtractor(CIterator_t begin, CIterator_t end, bool skipFirstWord);
  Result<std::string> extractQuotedString();
  bool hasQuotedString();
  Result<int> extractInt();
  Result<std::string> extractWord();
  bool hasWord();
  bool hasNum();
  bool eol();
  void skipWord();
  void skipWs();
  /// @brief You'd probably want to check for eol before usage
  bool isSpace();

private:
  CIterator_t cur_;
  CIterator_t end_;
  /// @param it must be between cur_ and end_
  bool eol(CIterator_t it);
};

#endif
