#include "JsonUtils.hpp"

Json::Value stringToJson(std::string_view s)
{
  Json::Value root;
  Json::CharReaderBuilder builder;
  const std::unique_ptr< Json::CharReader > reader(builder.newCharReader());
  if (!reader->parse(s.data(), s.data() + s.length(), &root, nullptr))
    throw Json::Exception("");
  return root;
}
