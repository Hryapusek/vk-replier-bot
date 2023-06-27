#ifndef API_REQUESTS_HPP
#define API_REQUESTS_HPP

#include <string>
#include <cpr/cpr.h>

namespace vk
{
  //TODO pause between requests
  //TODO add token and "v" to parameters

  class Requests
  {
  public:
    static void init(const std::string &token_, const std::string &v_);
    static cpr::Response sendRequest(const std::string &method, cpr::Parameters params);
  
  private:
    static const std::string baseUrl;
    static std::string token;
    static std::string v;
  };

}

#endif
