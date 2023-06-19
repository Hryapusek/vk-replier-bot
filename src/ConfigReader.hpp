#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <optional>
#include <string>
#include <vector>
#include "BlocksTable.hpp"

enum Mode
{
  WORK,
  CONFIG
};

struct Config
{
  Mode mode;
  std::string token;
  std::optional<BlocksTable> blocksTable;
  std::optional<int> sourceChatId;
  std::optional<std::vector<int>> statusCheckersId;
  std::string secret_string;
};

Config readConfigFromFile(const std::string& fileName);

#endif
