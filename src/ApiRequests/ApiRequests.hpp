#ifndef API_REQUESTS_HPP
#define API_REQUESTS_HPP

#include <string>
#include <cpr/cpr.h>

namespace vk
{
  const std::string baseUrl = "https://api.vk.com/method/";
  //TODO pause between requests
  //TODO add token and "v" to parameters
  cpr::Response sendRequest(const std::string &method, cpr::Parameters params);
}

#endif
