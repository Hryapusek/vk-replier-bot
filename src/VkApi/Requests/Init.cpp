#include "Init.hpp"
#include "BaseRequest.hpp"

namespace vk::requests
{
  void initRequests(std::string token, std::string version, std::string baseUrl)
  {
    details::BaseRequest::init(token, version, baseUrl);
  }

  void initRequests(std::string token, std::string version)
  {
    details::BaseRequest::init(token, version);
  }
}
