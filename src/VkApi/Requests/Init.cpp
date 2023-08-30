#include "Init.hpp"
#include "BaseRequest.hpp"

using str_cref = const std::string &;

void vk::requests::initRequests(str_cref token, str_cref version, str_cref baseUrl)
{
  details::BaseRequest::init(token, version, baseUrl);
}

void vk::requests::initRequests(str_cref token, str_cref version)
{
  details::BaseRequest::init(token, version);
}
