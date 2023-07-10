#ifndef BASE_REQUESTS_HPP
#define BASE_REQUESTS_HPP

#include <string>
#include <thread>
#include <jsoncpp/json/json.h>
#include <cpr/cpr.h>

namespace vk::requests::details
{
  class BaseRequest
  {
  public:
    using str_cref = const std::string &;
    static void init(str_cref token, str_cref v = "5.131", str_cref baseUrl = "https://api.vk.com/method");
    virtual ~BaseRequest() = default;

  protected:
    std::string method;
    cpr::Parameters params;
    Json::Value responseJson;
    BaseRequest(str_cref method);
    BaseRequest(std::string &&method);
    /// @throws Json::Exception - if bad json
    /// @throws RequestException - if failed after retries
    /// @note Does not throw anything if response.text is empty.
    void send();

  private:
    static std::string baseUrl;
    static cpr::Parameters generalParameters;
    static const std::vector< std::chrono::seconds > breakTimes;
    static std::thread pauseThread;
    static std::mutex pauseThreadMutex;
    /// @throws RequestException if failed after retries
    void retrySending();
    cpr::Response performRequest();
    static void waitForPauseBetweenRequests();
  };

}

#endif
