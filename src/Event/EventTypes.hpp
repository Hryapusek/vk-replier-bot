#ifndef EVENTTYPES_HPP
#define EVENTTYPES_HPP

enum EventType
{
  MESSAGE_NEW, //EventObject contains NewMessage.
  CONFIRMATION, //Vk requested confirmation.
  UNKNOWN //Event type was not recognized.
};

#endif
