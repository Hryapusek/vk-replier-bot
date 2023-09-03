#include "BusinessLogic/BusinessLogic.hpp"
#include "Server/Server.hpp"
#include "MessageProcessing/EventProcessingStrategy.hpp"
#include "Logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP


// 1. Put server in separate file
// 2. Incapsulate logger
// 3. Put error messages in separate class
// 4. Parse Event NOT EventType

// FIX - print TimeStamp, LineID, ProcessID, ThreadID
// OPTIMIZATION - config vectors to sets

void startServer()
{
  auto port = BusinessLogic::getPort();
  auto secretString = BusinessLogic::getSecretString();
  auto strategy = msg_proc::EventProcessingStrategy::make_strategy();
  auto server = server::Server::make_server(port, secretString, std::move(strategy));
  server->startServer(server);
  int a = 2;
  ++a;
}

int main() {
  BusinessLogic::init();
  startServer();
  return 0;
}