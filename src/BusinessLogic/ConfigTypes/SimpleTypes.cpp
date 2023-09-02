#include "SimpleTypes.hpp"
#include <map>
#include <string>

std::string config::types::modeToStr(Mode mode)
{
  static const const std::map< Mode, std::string > modeToString
  {
    { Mode::WORK, "WORK" },
    { Mode::CONFIG, "CONFIG" },
  };
  
  return modeToString.at(mode);
}