#define BOOST_LOG_DYN_LINK 1
#ifndef LOGGER_BOOST_IMPL_HPP
#define LOGGER_BOOST_IMPL_HPP

class Logger;

#include "Logger.hpp"
#include <boost/log/trivial.hpp>

class LoggerBoostImpl
{
  using Level_t = Logger::Level;

public:
  template < class ... Args >
  void log(Level_t lvl, Args &&... args);

  void init(std::string fileName);

private:
  ::boost::log::trivial::severity_level enumLvlToInnerLvl(Level_t lvl);
};

template < class ... Args >
void LoggerBoostImpl::log(Level_t lvl, Args &&... args)
{
  auto level = enumLvlToInnerLvl(lvl);
  BOOST_LOG_TRIVIAL(level) << ... << std::forward<Args>(args);
}

#endif