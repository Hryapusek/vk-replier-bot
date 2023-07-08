#ifndef TARGETS_TABLE_HPP
#define TARGETS_TABLE_HPP

#include <map>
#include "Chats.hpp"

class TargetsTable
{
  using Map = std::map<int, TargetChat>;
public:
  /// @param[in] chat Chat to insert. Chat.num will be used ad a key
  /// @return True if inserted successfully. False otherwise.
  bool insert(TargetChat chat);
  bool generateNumAndInsert(TargetChat chat);
  bool removeByPeerID(int peerId);
  bool removeByNum(int num);
  Map::iterator findByPeerId(int peerId);
  Map::iterator findByNum(int num);
  Map::const_iterator findByPeerId(int peerId) const;
  Map::const_iterator findByNum(int num) const;
  /// @return True if chat with given peer_id present somewhere in the table. False otherwise.
  bool containsPeerId(int peer_id) const;
  /// @return True if num is busy(present in the table). False otherwise.
  bool containsNum(int num) const;
  bool empty() const;
  const TargetChat &at(int num) const;
  TargetChat &at(int num);
  /// @return Underlying map
  std::map<int, TargetChat> &get() { return table; };

private:
  Map table;
};

#endif
