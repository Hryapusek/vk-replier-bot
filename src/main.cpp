#include <iostream>
#include <httpserver.hpp>
#include <thread>
#include "ConfigReader.hpp"
#include "JsonUtils.hpp"
#include "Event/EventUtils.hpp"
#include "MessageProcessing/MessageProcessing.hpp"

void processEvent(Json::Value root)
{
  //TODO try catch if json is fucked up
  Event event = json_utils::parseEvent(root);
  switch (event.getType())
  {
  case MESSAGE_NEW:
  {
    std::shared_ptr< NewMessage > newMessage = std::dynamic_pointer_cast< NewMessage >(event.getEventObject());
    if (!newMessage)
    {
      //TODO i cant even imagine how you would reach here but still...
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
  Json::Value root = json_utils::stringToJsonValue(req.get_content());
  if (root["type"].asString() == "confimation")
  {
    auto config = ConfigHolder::getReadOnlyConfig();
    return std::shared_ptr< httpserver::http_response >(new httpserver::string_response(config.config.secret_string, 200));
  }
  else
  {
    std::thread th(processEvent, root);
    th.detach();
  }
  return std::shared_ptr< httpserver::http_response >(new httpserver::string_response("Hello World!!!", 200));
}

int main() {
  httpserver::webserver ws = httpserver::create_webserver(8080);
  hello_world_resource hwr;
  ws.register_resource("/notification", &hwr, true);
  ws.start(true);
  return 0;
}