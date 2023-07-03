#ifndef VKAPI_EVENT_TYPES_HPP
#define VKAPI_EVENT_TYPES_HPP

namespace vk::callback::event
{
  enum class EventType
  {
    MESSAGE_NEW, //EventObject contains NewMessage.
    CONFIRMATION, //Vk requested confirmation.
    UNKNOWN //Event type was not recognized.
  };
}

#endif
