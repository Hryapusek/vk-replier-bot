#define BOOST_LOG_DYN_LINK 1
#include <iostream>
#include <thread>
#include <numeric>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <httpserver.hpp>

#include "ConfigReader.hpp"
#include "VkApi/Callback/Event/Event.hpp"
#include "JsonUtils.hpp"
#include "VkApi/Callback/Event/Utils.hpp"
#include "VkApi/Requests/BaseRequest.hpp"
#include "MessageProcessing/MessageProcessing.hpp"

// TODO api module separation
// TODO add namespaces in api module

std::vector< std::thread > threads;
std::mutex threadsMutex;

//erases finished threads every n seconds
void threadsCleaner()
{
  static const auto breakBetweenClenings = std::chrono::minutes(5);
  while (true)
  {
    std::this_thread::sleep_for(breakBetweenClenings);
    std::lock_guard< std::mutex > lock(threadsMutex);
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
      keywords::rotation_size = 10 * 1024 * 1024, //bytes
      keywords::auto_flush = true
    );
    add_console_log(std::cout);    

    core::get()->set_filter
    (
      trivial::severity >= trivial::trace
    );
  }
  config::ConfigHolder::readConfigFromFile("config.json");
  const auto &config =  config::ConfigHolder::getReadOnlyConfig().config;
  {
    using namespace vk::requests::details;
    if (config.baseUrl)
      BaseRequest::init(config.token, config.v, config.baseUrl.value());
    else
      BaseRequest::init(config.token, config.v);
  }
  std::thread th(threadsCleaner);
  th.detach();
}

template < class T, class ... Args >
void logArgs(T &&message, Args &&... args)
{
  BOOST_LOG_TRIVIAL(warning) << message;
  if constexpr (sizeof...(Args) != 0)
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
  using namespace vk::callback::event;
  using namespace vk::callback::event::objects;
  Event event;
  try
  {
    event = Event::fromJson(root);
  }
  catch (const Json::Exception &e)
  {
    logSkipEvent("Bad json found", root);
    return;
  }
  switch (event.getType())
  {
  case EventType::MESSAGE_NEW:
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
    logSkipEvent("Unrecognized event type found", root);
    return;
  }
}

class hello_world_resource: public httpserver::http_resource
{
public:
  std::shared_ptr< httpserver::http_response > render(const httpserver::http_request &);
};

std::shared_ptr< httpserver::http_response > hello_world_resource::render(const httpserver::http_request &req)
{
  using namespace config;
  using namespace vk::callback::event::details;
  using namespace vk::callback::event;
  Json::Value root;
  try
  {
    root = stringToJson(req.get_content());
  } catch (const Json::Exception &e)
  {
    logSkipEvent("Got bad json from vk", req.get_content());
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
  }
  switch (parseEventType(root))
  {
  case EventType::CONFIRMATION:
  {
    auto config = ConfigHolder::getReadOnlyConfig();
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response(config.config.secret_string, 200));
  }
  case EventType::MESSAGE_NEW:
  {
    threads.emplace_back(processEvent, std::move(root));
    break;
  }
  default:
  {
    logSkipEvent("Unrecognized event type found", root);
    break;
  }
  }
  return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
}

int main() {
  init();
  httpserver::webserver ws = httpserver::create_webserver(config::ConfigHolder::getReadOnlyConfig().config.port);
  hello_world_resource hwr;
  ws.register_resource("/notification", &hwr, true);
  ws.start(true);
  std::lock_guard< std::mutex > lock(threadsMutex);
  for (auto &th : threads)
    th.join();
  return 0;
}