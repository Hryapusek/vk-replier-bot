#ifndef VKAPI_INIT_REQUESTS
#define VKAPI_INIT_REQUESTS

#include <string>

namespace vk::requests
{
  void initRequests(std::string token, std::string version, std::string baseUrl);
  void initRequests(std::string token, std::string version);
}

#endif
