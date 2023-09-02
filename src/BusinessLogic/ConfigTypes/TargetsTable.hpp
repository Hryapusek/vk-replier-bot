#ifndef TARGETS_TABLE_HPP
#define TARGETS_TABLE_HPP

#include <map>
#include "SimpleTypes.hpp"

namespace config::types
{
  class TargetsContainer
  {
    using Container_t = std::map< ChatId_t, TargetChat_t >;
    using Iterator_t = Container_t::iterator;
    using CIterator_t = Container_t::const_iterator;
  public:
    /// @param[in] chat Chat to insert. Chat.num will be used as a key
    /// @return True if inserted successfully. False otherwise.
    bool insert(TargetChat_t chat);
    bool removeByVkChatId(VkChatId_t);
    bool removeByChatId(ChatId_t);
    Container_t::iterator findByVkChatId(VkChatId_t);
    Container_t::iterator findByChatId(ChatId_t);
    Container_t::const_iterator findByVkChatId(VkChatId_t) const;
    Container_t::const_iterator findByChatId(ChatId_t) const;
    bool containsVkChatId(VkChatId_t) const;
    bool containsChatId(ChatId_t) const;
    bool empty() const;
    const TargetChat_t &at(ChatId_t) const;
    TargetChat_t &at(ChatId_t);
    std::string getTargetIdsString() const;
    Iterator_t begin();
    Iterator_t end();
    CIterator_t begin() const;
    CIterator_t end() const;

  private:
    Container_t table_;
  };
}

#endif
