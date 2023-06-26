#include <iostream>
#include <httpserver.hpp>
#include <thread>
#include "ConfigReader.hpp"
#include "Event/Event.hpp"
#include "JsonUtils.hpp"
#include "MessageProcessing/MessageProcessing.hpp"

void init()
{
  config::ConfigHolder::readConfigFromFile("config.json");
}

void processEvent(Json::Value &&root)
{
  //TODO try catch if json is fucked up
  Event event;
  try
  {
    event = json_utils::parseEvent(root);
  }
  catch (const std::exception &e)
  {
    //TODO log bad json here
    return;
  }
  switch (event.getType())
  {
  case MESSAGE_NEW:
  {
    std::shared_ptr< NewMessage > newMessage = std::dynamic_pointer_cast< NewMessage >(event.getEventObject());
    if (!newMessage)
    {
      //TODO i cant even imagine how you would reach here but still...
      return;
    }
    commands::processMessage(newMessage);
    break;
  }
  default:
    //TODO logging
    break;
  }
}

class hello_world_resource: public httpserver::http_resource
{
public:
  std::shared_ptr< httpserver::http_response > render(const httpserver::http_request &);
};

std::shared_ptr< httpserver::http_response > hello_world_resource::render(const httpserver::http_request &req) {
  using namespace config;
  Json::Value root = json_utils::stringToJsonValue(req.get_content());
  switch (json_utils::parseEventType(root))
  {
  case CONFIRMATION:
  {
    auto config = ConfigHolder::getReadOnlyConfig();
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response(config.config.secret_string, 200));
  }
  case MESSAGE_NEW:
  {
    std::thread th(processEvent, std::move(root));
    th.detach();
  }
  default:
  {
    //TODO i cant even imagine...
  }
  }
  return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("Hello World!!!", 200));
}

int main() {
  init();
  httpserver::webserver ws = httpserver::create_webserver(8080);
  hello_world_resource hwr;
  ws.register_resource("/notification", &hwr, true);
  ws.start(true);
  return 0;
}