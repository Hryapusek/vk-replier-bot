#include "TargetsTable.hpp"
#include <limits>
#include <stdexcept>

bool TargetsTable::insert(TargetChat chat)
{
  if (containsNum(chat.num))
    return false;
  table[chat.num] = chat;
  return true;
}

bool TargetsTable::generateNumAndInsert(TargetChat chat)
{
  for (int num = std::numeric_limits<int>::min(); num < std::numeric_limits<int>::max(); ++num)
  {
    chat.num = num;
    if (!insert(chat))
      continue;
    return true;
  }
  return false;
}

bool TargetsTable::removeByPeerID(int peerId)
{
  auto it = findByPeerId(peerId);
  if (it == table.end())
    return false;
  table.erase(it);
  return true;
}

bool TargetsTable::removeByNum(int num)
{
  auto it = findByNum(num);
  if (it == table.end())
    return false;
  table.erase(it);
  return true;
}

TargetsTable::Map::iterator TargetsTable::findByPeerId(int peerId)
{
  for (auto beg = table.begin(); beg != table.end(); ++beg)
    if (beg->second.peer_id == peerId)
      return beg;
  return table.end();
}

TargetsTable::Map::iterator TargetsTable::findByNum(int num)
{
  return table.find(num);
}

TargetsTable::Map::const_iterator TargetsTable::findByPeerId(int peerId) const
{
  for (auto beg = table.cbegin(); beg != table.cend(); ++beg)
  {
    if (beg->second.peer_id == peerId)
    {
      return beg;
    }
  }
  return table.cend();
}

TargetsTable::Map::const_iterator TargetsTable::findByNum(int num) const
{
  return table.find(num);
}

bool TargetsTable::containsPeerId(int peer_id) const
{
  return findByPeerId(peer_id) != table.cend();
}

bool TargetsTable::containsNum(int num) const
{
  return findByNum(num) != table.cend();
}

bool TargetsTable::empty() const
{
  return table.empty();
}

const TargetChat &TargetsTable::at(int num) const
{
  return table.at(num);
}

TargetChat &TargetsTable::at(int num)
{
  return table.at(num);
}
