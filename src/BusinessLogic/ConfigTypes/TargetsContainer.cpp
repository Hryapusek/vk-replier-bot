#include "TargetsContainer.hpp"
#include <limits>
#include <stdexcept>

// TODO update realizations

namespace config::types
{
  void TargetsContainer::insert(TargetChat_t chat)
  {
    if (chat.chatId && containsChatId(*chat.chatId))
      throw std::logic_error("Precondition failed - given id is already busy. Id is " + std::to_string(*chat.chatId));
    else
      chat.chatId = generateChatId();
    table_[*chat.chatId] = chat;
  }

  bool TargetsContainer::removeByVkChatId(VkChatId_t vkChatId)
  {
    auto it = findByVkChatId(vkChatId);
    if (it == table_.end())
      throw std::logic_error("Precondition failed - no chat found with vkChatId " + std::to_string(vkChatId));
    table_.erase(it);
    return true;
  }

  bool TargetsContainer::removeByChatId(ChatId_t chatId)
  {
    auto it = findByChatId(chatId);
    if (it == table_.end())
      throw std::logic_error("Precondition failed - no chat found with chatId " + std::to_string(chatId));
    table_.erase(it);
    return true;
  }

  TargetsContainer::Iterator_t TargetsContainer::findByVkChatId(VkChatId_t vkChatId)
  {
    for (auto beg = table_.begin(); beg != table_.end(); ++beg)
      if (beg->second.vkChatId == vkChatId)
        return beg;
    return table_.end();
  }

  TargetsContainer::Iterator_t TargetsContainer::findByChatId(ChatId_t chatId)
  {
    return table_.find(chatId);
  }

  TargetsContainer::CIterator_t TargetsContainer::findByVkChatId(VkChatId_t vkChatId) const
  {
    for (auto beg = table_.cbegin(); beg != table_.cend(); ++beg)
      if (beg->second.vkChatId == vkChatId)
        return beg;
    return table_.cend();
  }

  TargetsContainer::CIterator_t TargetsContainer::findByChatId(ChatId_t chatId) const
  {
    return table_.find(chatId);
  }

  bool TargetsContainer::containsVkChatId(VkChatId_t vkChatId) const
  {
    return findByVkChatId(vkChatId) != table_.cend();
  }

  bool TargetsContainer::containsChatId(ChatId_t chatId) const
  {
    return findByChatId(chatId) != table_.cend();
  }

  bool TargetsContainer::empty() const
  {
    return table_.empty();
  }

  const TargetChat_t &TargetsContainer::at(ChatId_t num) const
  {
    return table_.at(num);
  }

  TargetChat_t &TargetsContainer::at(ChatId_t num)
  {
    return table_.at(num);
  }

  std::string TargetsContainer::getTargetIdsString() const
  {
    std::string result;
    for (auto beg = table_.cbegin(); beg != table_.cend(); ++beg)
      result += std::to_string(beg->second.vkChatId) + ',';
    if (!result.empty())
      result.pop_back();
    return result;
  }

  TargetsContainer::Iterator_t TargetsContainer::begin()
  {
    return table_.begin();
  }

  TargetsContainer::Iterator_t TargetsContainer::end()
  {
    return table_.end();
  }

  TargetsContainer::CIterator_t TargetsContainer::begin() const
  {
    return table_.cbegin();
  }

  TargetsContainer::CIterator_t TargetsContainer::end() const
  {
    return table_.cend();
  }

  ChatId_t TargetsContainer::generateChatId() const
  {
    static const auto MIN_ID = std::numeric_limits< ChatId_t >::min();
    static const auto MAX_ID = std::numeric_limits< ChatId_t >::max();
    for (ChatId_t freeChatId = MIN_ID; freeChatId < MAX_ID; ++freeChatId)
    {
      if (containsChatId(freeChatId))
        continue;
      return freeChatId;
    }
    throw std::logic_error("No free id left. Probably something bad had happen");
  }
}
