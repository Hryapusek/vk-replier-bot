#include "Server.hpp"
#include <httpserver.hpp>

#include <iostream>
#include <numeric>

#include "Logging/Logger.hpp"

#include "GeneralUtils/JsonUtils.hpp"
#include "VkApi/Callback/Event/Event.hpp"

namespace
{
  using namespace server;
  using Event_t = EventProcessingStrategyI::Event_t;

  Event_t stringToEvent(std::string_view str)
  {
    return Event_t::fromJson(stringToJson(str));
  }

  template < class ... Args >
  void logSkipEvent(Args &&... args)
  {
    Logger::log(Logger::WARNING, std::forward< Args >(args)..., "\nSkipping event");
  }
}

namespace server
{
  class HttpHandler: public httpserver::http_resource
  {
    friend class Server;
  public:
    HttpHandler(std::shared_ptr< Server > server);
    void processEvent(const httpserver::http_request &req, std::shared_ptr< httpserver::http_response > &resp);
    void walkThroughDefaultStrategies(bool (EventProcessingStrategyI::*processFunc)(Event_t event), Event_t event);
    std::shared_ptr< httpserver::http_response > render(const httpserver::http_request &);
  private:
    std::shared_ptr< Server > server_;
  };

  HttpHandler::HttpHandler(std::shared_ptr< Server > server) :
    server_(std::move(server))
  {  }

  // Pattern Strategy + Template Method
  void HttpHandler::processEvent(const httpserver::http_request &req, std::shared_ptr< httpserver::http_response > &resp)
  {
    using namespace vk::callback::event;
    Logger::log(Logger::INFO, "Got new request");
    if (server_->defaultEventProcesingStrategies_.empty() && server_->newsChatEventProcessingStrategies_.empty())
    {
      Logger::log(Logger::WARNING, "Strategies is not set. Skipping event.");
      return;
    }
    Event event;
    try
    {
      event = stringToEvent(req.get_content());
    }
    catch (const Json::Exception &e)
    {
      logSkipEvent("Bad event json: ", req.get_content());
      return;
    }
    switch (event.getType())
    {
      case EventType::CONFIRMATION:
      {
        Logger::log(Logger::INFO, "Event type is CONFIRMATION. Sending secret_string");
        resp = std::shared_ptr< httpserver::http_response >(new httpserver::string_response(server_->secretString_, 200));
        return;
      }
      case EventType::MESSAGE_NEW:
      {
        Logger::log(Logger::INFO, "Event type is MESSAGE_NEW. Starting eventProcess thread");
        server_->createThread(&HttpHandler::walkThroughDefaultStrategies, this, &EventProcessingStrategyI::processNewMessageEvent, std::move(event));
        return;
      }
      case EventType::UNKNOWN:
      {
        logSkipEvent("Unsupported event type", "Json:", req.get_content());
        return;
      }
      default:
      {
        logSkipEvent("eventType variable is not initialized", "Json:", req.get_content());
        return;
      }
    }
  }

  void HttpHandler::walkThroughDefaultStrategies(bool (EventProcessingStrategyI::*processFunc)(Event_t event), Event_t event)
  {
    for (auto &stratPrt : server_->defaultEventProcesingStrategies_)
      if (((*stratPrt).*processFunc)(std::move(event)))
        return;
  }

  std::shared_ptr< httpserver::http_response > HttpHandler::render(const httpserver::http_request &request)
  {
    auto response = std::shared_ptr< httpserver::http_response >(new httpserver::string_response("ok", 200));
    processEvent(request, response);
    return response;
  }

  Server::Server(int port, std::string secretString) :
    port_(port),
    secretString_(std::move(secretString))
  { }

  void Server::startServer(std::shared_ptr< Server > servPtr)
  {
    httpserver::webserver ws = httpserver::create_webserver(port_);
    HttpHandler handler(servPtr);
    ws.register_resource("/notification", &handler, true);
    std::thread cleanerTh(threadsCleanerDaemon, servPtr);
    cleanerTh.detach();
    bool blocking = true;
    ws.start(blocking);
  }

  void Server::addDefaultStrategy(std::shared_ptr< EventProcessingStrategyI > theStrategy)
  {
    if (!theStrategy)
      throw std::logic_error("Strategy can not be nullptr");
    defaultEventProcesingStrategies_.push_back(std::move(theStrategy));
  }

  std::shared_ptr< Server > Server::make_server(int port, std::string secretString)
  {
    return std::shared_ptr< Server >(new Server(port, std::move(secretString)));
  }

//erases finished threads time to time
  void Server::threadsCleanerDaemon(std::shared_ptr< Server > server)
  {
    static const auto breakBetweenCleanings = std::chrono::minutes(5);
    while (server.use_count() > 1)
    {
      std::this_thread::sleep_for(breakBetweenCleanings);
      std::lock_guard< std::mutex > lock(server->threadsMutex_);
      server->threads_.erase(
        std::remove_if(
          server->threads_.begin(),
          server->threads_.end(),
          [](std::thread &th) {
        return !th.joinable();
      }),
        server->threads_.end());
    }
  }
}
