#include "Event/EventObjects/EventObject.hpp"
#include "Event/EventObjects/NewMessage.hpp"
#include <memory>
#include <optional>

int main()
{
  std::shared_ptr<EventObject> obj;
  obj = std::make_shared<NewMessage>();
}
