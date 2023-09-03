#include "BusinessLogic/BusinessLogic.hpp"
#include "Server/Server.hpp"
#include "MessageProcessing/EventProcessingStrategy.hpp"

// 1. Put server in separate file
// 2. Incapsulate logger
// 3. Put error messages in separate class
// 4. Parse Event NOT EventType
// TODO fix regex, add \n to whitespaces

// FIX - print TimeStamp, LineID, ProcessID, ThreadID
// OPTIMIZATION - config vectors to sets
// TODO - NAMESPACE REPLIER

// TODO replace logger in VkApi module

void startServer()
{
  auto port = BusinessLogic::getPort();
  auto secretString = BusinessLogic::getSecretString();
  auto strategy = msg_proc::EventProcessingStrategy::make_strategy();
  auto server = server::Server::make_server(port, secretString, std::move(strategy));
  server->startServer();
}

int main() {
  BusinessLogic::init();
  startServer();
  return 0;
}