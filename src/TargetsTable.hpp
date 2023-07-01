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
  /// @param[in] num Chat's identificator.
  /// @param[in] chat Chat to insert.
  /// @return True if inserted successfully. False otherwise.
  bool insert(int num, Chat chat);
  bool insert(Chat chat);
  /// @return True if chat with given peer_id present somewhere in the table. False otherwise.
  bool containsPeerId(int peer_id) const;
  /// @return True if num is busy(present in the table). False otherwise.
  bool containsNum(int num) const;
  bool empty() const;
  const Chat &at(int num) const;
  Chat &at(int num);
  /// @return Underlying map
  std::map<int, Chat> &get() { return table; };

private:
  std::map<int, Chat> table;
};

#endif
