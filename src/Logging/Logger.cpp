#include "Logger.hpp"
#include <map>

std::shared_ptr<LoggerBoostImpl> Logger::theLogger = std::make_shared<LoggerBoostImpl>();

void Logger::init(std::string logFileName)
{
  theLogger->init(logFileName);
}
