#include "DelChecker.hpp"
#include <boost/log/trivial.hpp>
#include "../Utils.hpp"
#include "../../ConfigReader.hpp"

using namespace message_processing;
using namespace vk::requests::messages;
using namespace vk::objects;

namespace
{
  using namespace message_processing;
  using namespace message_processing::utils;
  using str_cref = const std::string &;
}

namespace message_processing::commands
{
  using namespace utils;
  void delChecker(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "delChecker";
    int checkerToDelete = message.getFromId();
    if (!checkMode(config::Mode::CONFIG, commandName, "Can not perform")
        || !checkIfChecker(checkerToDelete, commandName, "Can not perform. Not from checker. Skipping message"))
      return;
    
    {
      auto configWrap = ConfigHolder::getReadWriteConfig();
      auto &config = configWrap.config;
      auto &checkers = config.statusCheckersIds;
      auto it = std::find(checkers.begin(), checkers.end(), checkerToDelete);
      if (it != checkers.end())
        checkers.erase(it);
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully deleted checker " << checkerToDelete;
    MessagesSendRequest()
    .random_id(0)
    .peer_id(message.getPeerId())
    .message("Successfully deleted you as a checker!")
    .execute();
    return;
  }
}
