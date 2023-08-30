#include "Server.hpp"
#include <httpserver.hpp>

#include <iostream>
#include <numeric>

#include "../Logging/Logger.hpp"

#include "../JsonUtils.hpp"
#include "../VkApi/Callback/Event/Event.hpp"
#include "../MessageProcessing/MessageProcessing.hpp"

namespace
{
  using Event_t = vk::callback::event::Event;

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

class HttpHandler: public httpserver::http_resource
{
public:
  HttpHandler(std::shared_ptr< Server > server);
  void processEvent(const httpserver::http_request &req, std::shared_ptr< httpserver::http_response > &resp);
  std::shared_ptr< httpserver::http_response > render(const httpserver::http_request &);
private:
  std::shared_ptr< Server > server_;
};

HttpHandler::HttpHandler(std::shared_ptr< Server > server) :
  server_(std::move(server))
{ }

// Pattern Strategy + Template Method
void HttpHandler::processEvent(const httpserver::http_request &req, std::shared_ptr< httpserver::http_response > &resp)
{
  using namespace vk::callback::event;
  Logger::log(Logger::TRACE, "Got new request");
  if (server_->theEventProcessingStrategy_ == nullptr)
  {
    Logger::log(Logger::WARNING, "Strategy is not set. Skipping event.");
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
      Logger::log(Logger::TRACE, "Event type is CONFIRMATION. Sending secret_string");
      resp = std::shared_ptr< httpserver::http_response >(new httpserver::string_response(server_->secretString_, 200));
      return;
    }
    case EventType::MESSAGE_NEW:
    {
      Logger::log(Logger::TRACE, "Event type is MESSAGE_NEW. Starting eventProcess thread");
      server_->theEventProcessingStrategy_->processNewMessageEvent(std::move(event));
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

std::shared_ptr< httpserver::http_response > HttpHandler::render(const httpserver::http_request &request)
{
  auto response = std::shared_ptr< httpserver::http_response >(new httpserver::string_response("ok", 200));
  processEvent(request, response);
  return response;
}

Server::Server(int port, std::string secretString, std::shared_ptr<EventProcessingStrategyI> theStrategy) :
  port_(port),
  secretString_(std::move(secretString)),
  theEventProcessingStrategy_(std::move(theStrategy))
{ }

void Server::startServer()
{
  httpserver::webserver ws = httpserver::create_webserver(port_);
  HttpHandler handler(shared_from_this());
  ws.register_resource("/notification", &handler, true);
  std::thread cleanerTh(threadsCleanerDaemon, shared_from_this());
  cleanerTh.detach();
  bool blocking = true;
  ws.start(blocking);
}

std::shared_ptr<Server> Server::makeServer(int port, std::string secretString, std::shared_ptr<EventProcessingStrategyI> theStrategy)
{
  return std::make_shared< Server >(port, std::move(secretString), std::move(theStrategy));
}

//erases finished threads time to time
void Server::threadsCleanerDaemon(std::shared_ptr< Server > server)
{
  static const auto breakBetweenCleanings = std::chrono::minutes(5);
  while (server.use_count() > 1)
  {
    std::this_thread::sleep_for(breakBetweenCleanings);
    std::lock_guard< std::mutex > lock(threadsMutex_);
    threads_.erase(
      std::remove_if(
        threads_.begin(),
        threads_.end(),
        [](std::thread &th) {
      return !th.joinable();
    }),
      threads_.end());
  }
}
