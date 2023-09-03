#include "BusinessLogic/BusinessLogic.hpp"
#include "Server/Server.hpp"
#include "MessageProcessing/TagsCmdsProcessingStrategy.hpp"
#include "Logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#include "Logging/Logger.hpp"

void startServer()
{
  auto port = BusinessLogic::getPort();
  auto secretString = BusinessLogic::getSecretString();
  auto strategy = msg_proc::TagsCmdsProcessingStrategy::make_strategy();
  auto server = server::Server::make_server(port, secretString);
  server->addDefaultStrategy( std::move(strategy));
  server->startServer(server);
}

int main() {
  Logger::log(Logger::INFO, "Initializing...");
  BusinessLogic::init();
  Logger::log(Logger::INFO, "Starting server.");
  startServer();
  return 0;
}