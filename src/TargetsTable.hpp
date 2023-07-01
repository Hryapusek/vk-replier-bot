#ifndef TARGETS_TABLE_HPP
#define TARGETS_TABLE_HPP

#include <map>
#include <string>
#include <optional>

struct Chat
{
  int peer_id;
  std::optional<std::string> title;
};

class TargetsTable
{
public:
  /// @param[in] num Targets identificator.
  /// @param[in] target Target to insert.
  /// @return True if inserted successfully. False otherwise.
  bool insert(int num, Chat target);
  bool insert(Chat target);
  /// @return True if given Target.peer_id present somewhere in the table. False otherwise.
  bool containsTarget(Chat t) const;
  /// @return True if num is busy(present in the table). False otherwise.
  bool containsNum(int num) const;
  bool empty() const;
  int &at(int key) const;
  /// @return Underlying map
  std::map<int, Chat> &get() { return table; };

private:
  std::map<int, Chat> table;
};

#endif
