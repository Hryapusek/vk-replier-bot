#ifndef API_UTILS_HPP
#define API_UTILS_HPP

#include <string>
#include <vector>

namespace vk::requests::details
{
  template < class T >
  std::string vectorToString(std::vector< T > vect);

  template < class T >
  std::string vectorToString(std::vector< T > vect)
  {
    std::string res;
    for (const auto &el : vect)
    {
      res += std::to_string(el) + ',';
    }
    if (res.empty())
      return res;
    res.pop_back();
    return res;
  }
}

#endif
