#include "BusinessLogic/BusinessLogic.hpp"
#include "Server/Server.hpp"

// 1. Put server in separate file
// 2. Incapsulate logger
// 3. Put error messages in separate class
// 4. Parse Event NOT EventType
// TODO fix regex, add \n to whitespaces

// FIX - print TimeStamp, LineID, ProcessID, ThreadID
// OPTIMIZATION - config vectors to sets
// TODO - NAMESPACE REPLIER

// TODO replace logger in VkApi module

int main() {
  BusinessLogic::init();
  return 0;
}