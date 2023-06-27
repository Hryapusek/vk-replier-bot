#include "ApiRequests.hpp"

namespace vk
{
  const std::string Requests::baseUrl = "https://api.vk.com/method/";
  std::string Requests::token = "";
  std::string Requests::v = "";

  void Requests::init(const std::string &token_, const std::string &v_)
  { 
    token = token_;
    v = v_;
  }

  cpr::Response Requests::sendRequest(const std::string &method, cpr::Parameters params)
  {
    auto response = cpr::Get(cpr::Url{ baseUrl + method }, params);
    response.error.code != cpr::ErrorCode::OK;
  }
}
