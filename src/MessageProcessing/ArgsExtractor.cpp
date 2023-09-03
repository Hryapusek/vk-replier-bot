#include "ArgsExtractor.hpp"
#include "Logging/Logger.hpp"
#include <stdexcept>

ArgsExtractor::ArgsExtractor(CIterator_t begin, CIterator_t end, bool skipFirstWord) :
  cur_(begin),
  end_(end)
{ 
  if (skipFirstWord)
    skipWord();
}

Result<std::string> ArgsExtractor::extractQuotedString()
{
  if (!hasQuotedString())
    return make_error_result("Quoted string was not found");
  ++cur_;
  std::string quotedString;
  while (!eol() && *cur_ != '"')
    quotedString += *cur_++;
  if (eol() || *cur_ != '"')
    return make_error_result("Unclosed quote found");
  ++cur_;
  return make_success_result(std::move(quotedString));
}

bool ArgsExtractor::hasQuotedString()
{
  skipWs();
  return !eol() && *cur_ == '"';
}

Result<int> ArgsExtractor::extractInt()
{
  if (!hasNum())
    return make_error_result("Num is not found");
  std::string numWord = std::move(extractWord().getObject());
  try
  {
    auto num = std::stoi(numWord);
    return make_success_result(num);
  }
  catch(const std::exception& e)
  {
    return make_error_result("Bad number found");
  }
}

Result<std::string> ArgsExtractor::extractWord()
{
  if (!hasWord())
    return make_error_result("No words were found");
  std::string word;
  while (!eol() && !(isSpace()))
    word += *cur_++;
  return word;
}

bool ArgsExtractor::hasWord()
{
  skipWs();
  if (eol())
    return false;
  return true;
}

bool ArgsExtractor::hasNum()
{
  skipWs();
  if (eol())
    return false;
  auto next = std::next(cur_);
  bool isCurIsDigit = isdigit(*cur_);
  bool isCurIsSign = *cur_ == '-' || *cur_ == '+';
  bool isNextIsDigit = !eol(next) && isdigit(*next);
  return isCurIsDigit || (isCurIsSign && isNextIsDigit);
}

bool ArgsExtractor::eol()
{
  return cur_ == end_ || *cur_ == '\n';
}

void ArgsExtractor::skipWord()
{
  while (!eol() && !isSpace())
    ++cur_;
}

void ArgsExtractor::skipWs()
{
  while (!eol() && isSpace())
    ++cur_;
}

bool ArgsExtractor::isSpace()
{
  if (cur_ == end_)
  {
    Logger::log(Logger::FATAL, "Attempt to access end iterator in ArgsExtractor::isSpace");
    throw std::range_error("");
  }
  return *cur_ == ' ' || *cur_ == '\t';
}

bool ArgsExtractor::eol(CIterator_t it)
{
  return it == end_ || *it == '\n';
}
