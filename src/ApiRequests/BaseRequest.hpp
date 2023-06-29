#ifndef BASE_REQUESTS_HPP
#define BASE_REQUESTS_HPP

#include <string>
#include <thread>
#include <jsoncpp/json/json.h>
#include <cpr/cpr.h>

namespace vk
{
  class BaseRequest
  {
  public:
    using str_cref = const std::string &;
    BaseRequest(str_cref method);
    BaseRequest(std::string &&method);
    static void init(str_cref token, str_cref v = "5.131", str_cref baseUrl = "https://api.vk.com/method/");
    /// @throws Json::Exception - if bad json
    /// @throws RequestException - if failed after retries
    /// @note Does not throw anything if response.text is empty.
    void send();
    virtual ~BaseRequest() = default;

  protected:
    cpr::Parameters params;
    std::string method;
    Json::Value responseJson;
    static std::string baseUrl;
    static cpr::Parameters generalParameters;
    static const std::vector<std::chrono::seconds> breakTimes;
    static std::thread pauseThread;
    cpr::Response performRequest();
    static void waitForPauseBetweenRequests();

  private:
    /// @throws RequestException if failed after retries
    void retrySending();
  };

}

#endif
