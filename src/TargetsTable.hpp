#ifndef TARGETS_TABLE_HPP
#define TARGETS_TABLE_HPP

#include <map>
#include <string>
#include <optional>

struct Target
{
  int peer_id;
  std::optional<std::string> title;
};

class TargetsTable
{
public:
  /// @brief Inserts pair in table.
  /// @param[in] num Targets identificator.
  /// @param[in] target Target to insert.
  /// @return True if inserted successfully. False otherwise.
  bool insert(int num, Target target);
  bool insert(Target target, bool checkIfTargetPresent = false);
  /// @param[in] t
  /// @return True if given Target.peer_id present somewhere in the table. False otherwise.
  bool containsTarget(Target t) const;
  bool containsNum(int num) const;
  bool empty() const;
  int &at(int key) const;
  /// @return Underlying map
  std::map<int, Target> &get() { return table; };

private:
  std::map<int, Target> table;
};

#endif
