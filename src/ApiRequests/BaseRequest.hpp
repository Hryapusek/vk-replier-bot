#ifndef BASE_REQUESTS_HPP
#define BASE_REQUESTS_HPP

#include <string>
#include <cpr/cpr.h>

namespace vk
{
  //TODO pause between requests
  //TODO add token and "v" to parameters

  class BaseRequest
  {
    using str_cref = const std::string &;
  public:
    //TODO add general parameters in constructor
    static void init(str_cref token, str_cref v = "5.131", str_cref baseUrl = "https://api.vk.com/method/");
    void send();
    virtual ~BaseRequest() = default;

  protected:
    cpr::Parameters params;
    std::string method;
    Json::Value responseJson;
    static std::string baseUrl;
    static cpr::Parameters generalParameters;
    static const std::vector<std::chrono::seconds> breakTimes;
    cpr::Response performRequest();

  private:
    void retrySending();
  };

}

#endif
