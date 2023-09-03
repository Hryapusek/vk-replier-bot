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
    Server &operator=(const Server &) = delete;
    void startServer(std::shared_ptr< Server > servPtr);
    void addDefaultStrategy(std::shared_ptr< EventProcessingStrategyI > theStrategy);
    static std::shared_ptr< Server > make_server(int port, std::string secretString);

  protected:
    int port_;
    std::string secretString_;
    template < class ... Args >
    void createThread(Args &&...);
    Server(int port, std::string secretString);

  private:
    std::vector< std::thread > threads_;
    std::mutex threadsMutex_;
    std::vector< std::shared_ptr< EventProcessingStrategyI > > defaultEventProcesingStrategies_;
    std::vector< std::shared_ptr< EventProcessingStrategyI > > newsChatEventProcessingStrategies_;
    static void threadsCleanerDaemon(std::shared_ptr< Server >);
  };
  
  template <class... Args>
  inline void Server::createThread(Args &&... args)
  {
    std::unique_lock lock(this->threadsMutex_);
    this->threads_.emplace_back(std::forward<Args>(args)...);
  }
}

#endif
