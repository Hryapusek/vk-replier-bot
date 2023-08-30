#include "RegGodlike.hpp"
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
  void regGodlike(const Message &message, size_t pos)
  {
    using namespace config;
    static str_cref commandName = "regGodlike";
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfGodlike(message.getFromId(), req, commandName, "Can not perform command. Not from godlike"))
      return;
    int newGodlike;
    try
    {
      newGodlike = extractNum(message.getText(), pos);
    }
    catch (const std::logic_error &e)
    {
      logAndSendErrorMessage(req, commandName, "Can not perform command. Incorrect number passed");
      return;
    }
    if (!checkIfNotGodlike(newGodlike, req, commandName, "Can not perform command. User already godlike"))
      return;
    {
      auto configWrap = ConfigHolder::getReadWriteConfig();
      auto &config = configWrap.config;
      config.godlikeIds.push_back(newGodlike);
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully added new godlike " << newGodlike;
    req.message("Successfully added new godlike " + std::to_string(newGodlike)).execute();
    return;
  }
}
