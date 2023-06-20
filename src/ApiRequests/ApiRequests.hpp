#ifndef API_REQUESTS_HPP
#define API_REQUESTS_HPP

#include <string>
#include <cpr/cpr.h>

namespace vk
{
  const std::string baseUrl = "https://api.vk.com/method/";
  void sendRequest(const std::string &method, cpr::Parameters params);
}

#endif
