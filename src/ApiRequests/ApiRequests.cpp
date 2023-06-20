#include "ApiRequests.hpp"

namespace vk
{
  void sendRequest(const std::string &method, const cpr::Parameters& params)
  {
    auto response = cpr::Get(cpr::Url{ baseUrl + method }, params);
    response.error.code != cpr::ErrorCode::OK;
  }
}
