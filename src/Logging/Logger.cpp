#include "Logger.hpp"
#include <map>

using namespace details_;

std::shared_ptr< LoggerEasyLogImpl > Logger::theLogger = std::make_shared< LoggerEasyLogImpl >();

void Logger::init(std::string logFileName)
{
  theLogger->init(logFileName);
}

void LoggerEasyLogImpl::init(std::string fileName)
{
  el::Configurations defaultConf;
  defaultConf.setToDefault();
  // Values are always std::string
  defaultConf.set(el::Level::Global,
    el::ConfigurationType::Format, "%datetime %level %thread %msg");
  defaultConf.set(el::Level::Global, el::ConfigurationType::Filename, fileName);
  defaultConf.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "10485760");
  el::Loggers::reconfigureLogger("default", defaultConf);
}

el::Level LoggerEasyLogImpl::enumLvlToInnerLvl(Level_t lvl)
{
  static const std::map< Level_t, el::Level > enumLvlToBoostLvlMap
  {
    { Logger::TRACE,      el::Level::Trace },
    { Logger::DEBUG,      el::Level::Debug },
    { Logger::INFO,        el::Level::Info },
    { Logger::WARNING,  el::Level::Warning },
    { Logger::ERROR,      el::Level::Error },
    { Logger::FATAL,      el::Level::Fatal }
  };
  return enumLvlToBoostLvlMap.at(lvl);
}
