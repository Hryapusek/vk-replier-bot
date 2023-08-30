#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

class LoggerBoostImpl;

#include "LoggerBoostImpl.hpp"

class Logger
{
public:
  enum Level
  {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
  };

  template < class ... Args >
  static void log(Level lvl, Args &&... args);

  static void init(std::string logFileName);

private:
  static std::shared_ptr<LoggerBoostImpl> theLogger;
};

#endif

template <class... Args>
inline void Logger::log(Level lvl, Args &&...args)
{
  theLogger->log(lvl, std::forward(args));
}
