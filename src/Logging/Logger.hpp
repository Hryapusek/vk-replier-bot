#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

namespace details_
{
  class LoggerEasyLogImpl;
}

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
  static std::shared_ptr< details_::LoggerEasyLogImpl > theLogger;
};

#include "easylogging++.h"

namespace details_
{
  class LoggerEasyLogImpl
  {
    using Level_t = Logger::Level;

  public:
    template < class ... Args >
    void log(Level_t lvl, Args &&... args);

    void init(std::string fileName);

  private:
    el::Level enumLvlToInnerLvl(Level_t lvl);
  };

  template < class ... Args >
  void LoggerEasyLogImpl::log(Level_t lvl, Args &&... args)
  {
    auto level = enumLvlToInnerLvl(lvl);
    switch (level)
    {
      case el::Level::Trace:
        (LOG(TRACE) << ... << std::forward< Args >(args));
        break;

      case el::Level::Debug:
        (LOG(DEBUG) << ... << std::forward< Args >(args));
        break;

      case el::Level::Info:
        (LOG(INFO) << ... << std::forward< Args >(args));
        break;

      case el::Level::Warning:
        (LOG(WARNING) << ... << std::forward< Args >(args));
        break;

      case el::Level::Error:
        (LOG(ERROR) << ... << std::forward< Args >(args));
        break;

      case el::Level::Fatal:
        (LOG(FATAL) << ... << std::forward< Args >(args));
        break;
    }
  }
}

template < class ... Args >
void Logger::log(Logger::Level lvl, Args &&... args)
{
  theLogger->log(lvl, std::forward< Args >(args)...);
}

#endif
