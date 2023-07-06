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

// TODO add trace logging
// TODO print TimeStamp, LineID, ProcessID, ThreadID
// TODO make clearer log position(from where it came)

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

void initLogger()
{
  using namespace boost::log;
  add_file_log
  (
    keywords::file_name = "logfile.log",
    keywords::rotation_size = 10 * 1024 * 1024,   //bytes
    keywords::auto_flush = true
  );
  add_console_log(std::cout);
  core::get()->set_filter(trivial::severity >= trivial::trace);
  add_common_attributes();
}

void init()
{
  using namespace config;
  initLogger();
  ConfigHolder::readConfigFromFile("config.json");
  {
    using namespace vk::requests::details;
    if (ConfigHolder::hasBaseUrl())
      BaseRequest::init(ConfigHolder::getToken(), ConfigHolder::getV(), ConfigHolder::getBaseUrl());
    else
      BaseRequest::init(ConfigHolder::getToken(), ConfigHolder::getV());
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
    logSkipEvent("Can not get Event from json", e.what(), "Json:", root);
    return;
  }
  switch (event.getType())
  {
    case EventType::MESSAGE_NEW:
    {
      std::shared_ptr< NewMessage > newMessage = std::dynamic_pointer_cast< NewMessage >(event.getEventObject());
      if (!newMessage)
      {
        logSkipEvent("Can not cast EventObject to NewMessage", "Json:", root);
        return;
      }
      commands::processMessage(newMessage);
      break;
    }
    case EventType::UNKNOWN:
    {
      logSkipEvent("Can't even imagine how you get unknown event in processEvent");
      return;
    }
    default:
    {
      logSkipEvent("Can't even imagine how you get uninitialized event.getType() in processEvent");
      return;
    }
  }
}

class HttpHandler: public httpserver::http_resource
{
public:
  std::shared_ptr< httpserver::http_response > render(const httpserver::http_request &);
};

std::shared_ptr< httpserver::http_response > HttpHandler::render(const httpserver::http_request &req)
{
  using namespace config;
  using namespace vk::callback::event::details;
  using namespace vk::callback::event;
  BOOST_LOG_TRIVIAL(info) << "Got new event";
  Json::Value root;
  try
  {
    root = stringToJson(req.get_content());
  } catch (const Json::Exception &e)
  {
    logSkipEvent("Bad json in request body", "Content:", req.get_content());
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
  }
  if (!root.isMember("type"))
  {
    logSkipEvent("\"type\" field was not found", "Json:", root);
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
  }
  EventType eventType;
  try
  {
    eventType = parseEventType(root);
  }
  catch (const Json::Exception &e)
  {
    logSkipEvent("Incorrect \"type\" field type", "Json:", root);
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
  }
  switch (eventType)
  {
    case EventType::CONFIRMATION:
    {
      BOOST_LOG_TRIVIAL(info) << "Event type is CONFIRMATION. Sending secret_string";
      return std::shared_ptr< httpserver::http_response >(new httpserver::string_response(ConfigHolder::getSecretString(), 200));
    }
    case EventType::MESSAGE_NEW:
    {
      BOOST_LOG_TRIVIAL(info) << "Event type is MESSAGE_NEW. Starting eventProcess thread";
      threads.emplace_back(processEvent, std::move(root));
      break;
    }
    case EventType::UNKNOWN:
    {
      logSkipEvent("Unsupported event type", "Json:", root);
      break;
    }
    default:
    {
      logSkipEvent("eventType variable is not initialized", "Json:", root);
      break;
    }
  }
  return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("", 200));
}

int main() {
  init();
  httpserver::webserver ws = httpserver::create_webserver(config::ConfigHolder::getPort());
  HttpHandler handler;
  ws.register_resource("/notification", &handler, true);
  ws.start(true);
  std::lock_guard< std::mutex > lock(threadsMutex);
  for (auto &th : threads)
    th.join();
  return 0;
}