#ifndef VKAPI_NEWMESSAGE_HPP
#define VKAPI_NEWMESSAGE_HPP

#include "EventObject.hpp"

#include <string>
#include "../../../Objects/Message.hpp"
#include <jsoncpp/json/json.h>

namespace vk::callback::event::objects
{
  using namespace vk::objects;

/**
 * @brief Class for message_new event type. It contains only
 * one Message object.
 */
  class NewMessage: public EventObject
  {
  public:
    Message &getMessage() {
      return message;
    }
    const Message &getMessage() const {
      return message;
    }
    /// @throw Json::Exception
    static NewMessage fromJson(std::reference_wrapper< Json::Value >);

  private:
    Message message;
    void foo() override { }
  };
}

#endif
