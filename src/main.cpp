#define BOOST_LOG_DYN_LINK 1
#include <iostream>
#include <thread>
#include <numeric>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <httpserver.hpp>

#include "ConfigReader.hpp"
#include "Event/Event.hpp"
#include "JsonUtils.hpp"
#include "ApiRequests/BaseRequest.hpp"
#include "MessageProcessing/MessageProcessing.hpp"

std::vector< std::thread > threads;
std::mutex threadsMutex;

//erases finished threads every n seconds
void threadsCleaner()
{
  static const auto breakBetweenClenings = std::chrono::minutes(5);
  while (true)
  {
    std::this_thread::sleep_for(breakBetweenClenings);
    std::lock_guard<std::mutex> lock(threadsMutex);
    threads.erase(
      std::remove_if(
        threads.begin(),
        threads.end(),
        [](std::thread &th) {
      return !th.joinable();
    }),
      threads.end());
  }
}

void init()
{
  {
    using namespace boost::log;
    add_file_log
    (
      keywords::file_name = "logfile.log",
      keywords::rotation_size = 10 * 1024 * 1024 //bytes
    );

    core::get()->set_filter
    (
      trivial::severity >= trivial::trace
    );
  }
  // TODO add try catch
  config::ConfigHolder::readConfigFromFile("config.json");
  const auto &config =  config::ConfigHolder::getReadOnlyConfig().config;
  if (config.baseUrl)
    vk::BaseRequest::init(config.token, config.v, config.baseUrl.value());
  else
    vk::BaseRequest::init(config.token, config.v);
  std::thread th(threadsCleaner);
  th.detach();
}

template < class T, class ... Args >
void logArgs(T &&message, Args &&... args)
{
  BOOST_LOG_TRIVIAL(warning) << message;
  logArgs(std::forward< Args >(args)...);
}

template < class T >
void logArgs(T &&message)
{
  BOOST_LOG_TRIVIAL(warning) << message;
}

template < class ... Args >
void logSkipEvent(Args &&... args)
{
  logArgs(std::forward< Args >(args)...);
  BOOST_LOG_TRIVIAL(warning) << "Skipping event";
}

void processEvent(Json::Value &&root)
{
  Event event;
  try
  {
    event = json_utils::parseEvent(root);
  }
  catch (const Json::Exception &e)
  {
    logSkipEvent("Bad json found", root);
    return;
  }
  switch (event.getType())
  {
  case MESSAGE_NEW:
  {
    std::shared_ptr< NewMessage > newMessage = std::dynamic_pointer_cast< NewMessage >(event.getEventObject());
    if (!newMessage)
    {
      logSkipEvent("Can not convert EventObject to newMessage", root);
      return;
    }
    commands::processMessage(newMessage);
    break;
  }
  default:
    logSkipEvent("Unsupported event", root);
    return;
  }
}

class hello_world_resource: public httpserver::http_resource
{
public:
  std::shared_ptr< httpserver::http_response > render(const httpserver::http_request &);
};

std::shared_ptr< httpserver::http_response > hello_world_resource::render(const httpserver::http_request &req) {
  using namespace config;
  Json::Value root;
  try
  {
    Json::Value root = json_utils::stringToJsonValue(req.get_content());
  } catch (const Json::Exception &e)
  {
    logSkipEvent("Got bad json from vk", req.get_content());
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
  }
  switch (json_utils::parseEventType(root))
  {
  case CONFIRMATION:
  {
    auto config = ConfigHolder::getReadOnlyConfig();
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response(config.config.secret_string, 200));
  }
  case MESSAGE_NEW:
  {
    threads.emplace_back(processEvent, std::move(root));
  }
  default:
  {
    logSkipEvent("Unrecognized event type found", root);
  }
  }
  return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
}

int main() {
  init();
  httpserver::webserver ws = httpserver::create_webserver(8080);
  hello_world_resource hwr;
  ws.register_resource("/notification", &hwr, true);
  ws.start(true);
  std::lock_guard<std::mutex> lock(threadsMutex);
  for (auto &th : threads)
    th.join();
  return 0;
}