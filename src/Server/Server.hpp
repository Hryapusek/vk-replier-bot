#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include "EventProcessingStrategyI.hpp"

namespace server
{
//Patter Facade
  class Server
  {
    friend class HttpHandler;
  public:
    Server(const Server &) = delete;
    Server(Server &&) = delete;
    Server &operator=(const Server &) = delete;
    Server &operator=(Server &&) = delete;
    static std::shared_ptr< Server > make_server(int port, std::string secretString, std::shared_ptr< EventProcessingStrategyI > theStrategy);
    void startServer(std::shared_ptr<Server> servPtr);

  protected:
    int port_;
    std::string secretString_;
    Server(int port, std::string secretString, std::shared_ptr< EventProcessingStrategyI > theStrategy);

  private:
    std::vector< std::thread > threads_;
    std::mutex threadsMutex_;
    std::shared_ptr< EventProcessingStrategyI > theEventProcessingStrategy_;
    static void threadsCleanerDaemon(std::shared_ptr< Server >);
  };
}

#endif
