#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class RequestException : std::exception
{
public:
  RequestException(const char* message);
  const char* what() const noexcept;

private:
  const char* message_;
};

#endif
