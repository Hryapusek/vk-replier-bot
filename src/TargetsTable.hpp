#ifndef TARGETS_TABLE_HPP
#define TARGETS_TABLE_HPP

#include <map>
#include "Chats.hpp"

class TargetsTable
{
public:
  /// @param[in] num Chat's identificator.
  /// @param[in] chat Chat to insert.
  /// @return True if inserted successfully. False otherwise.
  /// @throw std::logic_error is chat.num and num are not equal.
  bool insert(int num, TargetChat chat);
  bool insert(TargetChat chat);
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
  std::map<int, TargetChat> table;
};

#endif
