#include "Exceptions.hpp"

namespace vk::exceptions
{
  RequestException::RequestException(const char *message) :
    message_(message)
  { }

  const char *RequestException::what() const noexcept
  {
    return message_;
  }
}
