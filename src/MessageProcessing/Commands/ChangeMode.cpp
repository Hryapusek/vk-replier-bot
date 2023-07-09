#include "ChangeMode.hpp"
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
  void changeMode(const Message &message)
  {
    using namespace config;
    static str_cref commandName = "changeMode";
    auto currentMode = ConfigHolder::getReadOnlyConfig().config.mode;
    Mode newMode;
    if (currentMode == Mode::CONFIG)
      newMode = Mode::WORK;
    else
      newMode = Mode::CONFIG;
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfGodlike(message.getFromId(), commandName, "Can not perform command. Not from godlike")
        || !checkIfCanChangeModeTo(newMode, req, commandName, "Can not change mode. Some conditions are false"))
      return;
    {
      auto configWrap = ConfigHolder::getReadWriteConfig();
      auto &config = configWrap.config;
      if (config.mode == Mode::CONFIG)
        config.mode = Mode::WORK;
      else
        config.mode = Mode::CONFIG;
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully switched mode to " << modeToString.at(newMode);
    req.message("Successfully switched mode to " + modeToString.at(newMode)).execute();
    return;
  }
}
