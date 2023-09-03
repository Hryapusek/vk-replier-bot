#include "BaseRequest.hpp"
#include "Logging/Logger.hpp"
#include "VkApi/Exceptions.hpp"
#include "GeneralUtils/JsonUtils.hpp"

namespace
{
  void pauseBetweenRequests()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void logHttpError(long status_code)
  {
    Logger::log(Logger::WARNING, "Http error occured sending request. Code " + std::to_string(status_code));
  }

  void logVkApiError(const Json::Value &responseJson)
  {
    Logger::log(Logger::WARNING, "VK API error occured sending request.\n", responseJson);
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
    return stringToJson(responseText);
  }
}

namespace vk::requests::details
{
  std::string BaseRequest::baseUrl = "";
  cpr::Parameters BaseRequest::generalParameters = cpr::Parameters{ };
  const std::vector< std::chrono::seconds > BaseRequest::breakTimes = {
    std::chrono::seconds(1),
    std::chrono::seconds(2),
    std::chrono::seconds(5)
  };
  std::thread BaseRequest::pauseThread = std::thread();
  std::mutex BaseRequest::pauseThreadMutex = std::mutex();

  BaseRequest::BaseRequest(str_cref method) :
    method(method),
    params(generalParameters)
  { }

  BaseRequest::BaseRequest(std::string &&method) :
    method(std::move(method)),
    params(generalParameters)
  { }

  void BaseRequest::init(str_cref token, str_cref v, str_cref baseUrl)
  {
    BaseRequest::baseUrl = baseUrl;
    BaseRequest::generalParameters.Add({ { "access_token", token }, { "v", v } });
  }

  void BaseRequest::send()
  {
    Logger::log(Logger::INFO, "Sending request.");
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
    Logger::log(Logger::INFO, "Sending request completed successfully.");
    return;
  }

  cpr::Response BaseRequest::performRequest()
  {
    using namespace std::chrono_literals;
    waitForPauseBetweenRequests();
    return cpr::Get(cpr::Url{ baseUrl + "/" + method }, params, cpr::Timeout{ 10s });
  }

  void BaseRequest::waitForPauseBetweenRequests()
  {
    std::unique_lock< std::mutex > lock(pauseThreadMutex);
    if (pauseThread.joinable())
      pauseThread.join();
    pauseThread = std::thread(pauseBetweenRequests);
  }

  void BaseRequest::retrySending()
  {
    cpr::Response response;
    int nTry = 0;
    for (auto breakTime : breakTimes)
    {
      Logger::log(Logger::WARNING, "Sleeping before retry...");
      std::this_thread::sleep_for(breakTime);
      ++nTry;
      Logger::log(Logger::WARNING, "Retry number ", std::to_string(nTry), " to send request.");
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
    Logger::log(Logger::ERROR, "Out of retry sendings. Throwing RequestException.");
    throw exceptions::RequestException("Failed after retries");
  }
}
