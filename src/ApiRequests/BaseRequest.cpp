#define BOOST_LOG_DYN_LINK 1
#include "BaseRequest.hpp"
#include <boost/log/trivial.hpp>
#include "Exceptions.hpp"
#include "../JsonUtils.hpp"

namespace
{
  void pauseBetweenRequests()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void logHttpError(long status_code)
  {
    BOOST_LOG_TRIVIAL(warning) << "Http error occured sending request. Code " << status_code;
  }

  void logVkApiError(const Json::Value &responseJson)
  {
    BOOST_LOG_TRIVIAL(warning) << "VK API error occured sending request.\n" << responseJson;
  }

  bool isVkError(const Json::Value &responseJson)
  {
    return responseJson.isMember("error");
  }

  bool isHttpError(const cpr::Response &response)
  {
    return response.error.code != cpr::ErrorCode::OK;
  }

  Json::Value getJsonFromResponse(const std::string &responseText)
  {
    if (responseText.empty())
    {
      return Json::Value();
    }
    return json_utils::stringToJsonValue(responseText);
  }
}

namespace vk
{
  std::string BaseRequest::baseUrl = "";
  cpr::Parameters BaseRequest::generalParameters = cpr::Parameters{ };
  const std::vector< std::chrono::seconds > BaseRequest::breakTimes = {
    std::chrono::seconds(1),
    std::chrono::seconds(2),
    std::chrono::seconds(5)
  };
  std::thread BaseRequest::pauseThread = std::thread();

  void BaseRequest::init(str_cref token, str_cref v, str_cref baseUrl)
  {
    BaseRequest::baseUrl = baseUrl;
    BaseRequest::generalParameters.Add({ { "access_token", token }, { "v", v } });
  }

  /// @throws Json::Exception - if bad json
  /// @throws RequestException - if failed after retries
  /// @note Does not throw anything if response.text is empty.
  void BaseRequest::send()
  {
    BOOST_LOG_TRIVIAL(info) << "Sending request.";
    auto response = performRequest();
    if (isHttpError(response))
    {
      logHttpError(response.status_code);
      retrySending();
      return;
    }
    responseJson = getJsonFromResponse(response.text);
    if (isVkError(responseJson))
    {
      logVkApiError(responseJson);
      retrySending();
      return;
    }
    BOOST_LOG_TRIVIAL(info) << "Sending request completed successfully.";
    return;
  }

  cpr::Response BaseRequest::performRequest()
  {
    using namespace std::chrono_literals;
    waitForPauseBetweenRequests();
    return cpr::Get(cpr::Url{ baseUrl + method }, params, cpr::Timeout{ 10s });
  }

  void BaseRequest::waitForPauseBetweenRequests()
  {
    if (pauseThread.joinable())
      pauseThread.join();
    pauseThread = std::thread(pauseBetweenRequests);
  }

  /// @throws RequestException if failed after retries
  void BaseRequest::retrySending()
  {
    cpr::Response response;
    int nTry = 0;
    for (auto breakTime : breakTimes)
    {
      BOOST_LOG_TRIVIAL(warning) << "Sleeping before retry...";
      std::this_thread::sleep_for(breakTime);
      ++nTry;
      BOOST_LOG_TRIVIAL(warning) << "Retry number " << nTry << " to send request.";
      response = performRequest();
      if (isHttpError(response))
      {
        logHttpError(response.status_code);
        continue;
      }
      responseJson = getJsonFromResponse(response.text);
      if (isVkError(responseJson))
      {
        logVkApiError(responseJson);
        continue;
      }
      return;
    }
    BOOST_LOG_TRIVIAL(error) << "Out of retry sendings. Throwing RequestException.";
    throw RequestException("Failed after retries");
  }
}
