#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include "EventProcessingStrategyI.hpp"

//Patter Facade
class Server : std::enable_shared_from_this<Server>
{
  friend class HttpHandler;
public:
  Server(const Server&) = delete;
  Server(Server&&) = delete;
  Server &operator=(const Server&) = delete;
  Server &operator=(Server&&) = delete;
  std::shared_ptr<Server> makeServer(int port, std::string secretString, std::shared_ptr<EventProcessingStrategyI> theStrategy);
  void startServer();

protected:
  int port_;
  std::string secretString_;
  Server(int port, std::string secretString, std::shared_ptr<EventProcessingStrategyI> theStrategy);

private:
  std::vector< std::thread > threads_;
  std::mutex threadsMutex_;
  std::shared_ptr<EventProcessingStrategyI> theEventProcessingStrategy_;
  void threadsCleanerDaemon(std::shared_ptr<Server>);
};
#endif