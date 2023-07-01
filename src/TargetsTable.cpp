#include "TargetsTable.hpp"
#include <limits>
#include <stdexcept>

bool TargetsTable::insert(int num, Chat chat)
{
  if (containsNum(num))
    return false;
  table[num] = chat;
  return true;
}

bool TargetsTable::insert(Chat chat)
{
  for (int num = std::numeric_limits<int>::min(); num < std::numeric_limits<int>::max(); ++num)
  {
    if (!insert(num, chat))
      continue;
    return true;
  }
  return false;
}

bool TargetsTable::containsPeerId(int peer_id) const
{
  for (const auto &pair : table)
  {
    if (pair.second.peer_id == peer_id)
      return true;
  }
  return false;
}

bool TargetsTable::containsNum(int num) const
{
  try
  {
    this->at(num);
    return true;
  }
  catch (const std::out_of_range &e)
  {
    return false;
  }
}

bool TargetsTable::empty() const
{
  return table.empty();
}

const Chat &TargetsTable::at(int num) const
{
  return table.at(num);
}

Chat &TargetsTable::at(int num)
{
  return table.at(num);
}
