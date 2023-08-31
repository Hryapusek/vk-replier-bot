#include "LoggerBoostImpl.hpp"

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

void LoggerBoostImpl::init(std::string fileName)
{
  using namespace boost::log;
  add_file_log
  (
    keywords::file_name = std::move(fileName),
    keywords::rotation_size = 10 * 1024 * 1024,   //bytes
    keywords::auto_flush = true
  );
  add_console_log(std::cout);
  core::get()->set_filter(trivial::severity >= trivial::trace);
  add_common_attributes();
}

::boost::log::trivial::severity_level LoggerBoostImpl::enumLvlToInnerLvl(Level_t lvl)
{
  using namespace ::boost::log::trivial;
  static const std::map<Level_t, ::boost::log::trivial::severity_level> enumLvlToBoostLvlMap
  {
    { Logger::TRACE,      trace },
    { Logger::DEBUG,      debug },
    { Logger::INFO,        info },
    { Logger::WARNING,  warning },
    { Logger::ERROR,      error },
    { Logger::FATAL,      fatal }
  };
  return enumLvlToBoostLvlMap.at(lvl);
}
