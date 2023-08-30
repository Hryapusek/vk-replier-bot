#include "DelTargetByNum.hpp"
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
  void delTargetByNum(const Message &message, size_t pos)
  {
    using namespace config;
    static str_cref commandName = "delTargetByNum";
    MessagesSendRequest req;
    req.random_id(0).peer_id(message.getPeerId());
    if (!checkIfGodlike(message.getFromId(), req, commandName, "Can not perform command. Not from godlike"))
      return;
    int targetNumToDelete;
    try
    {
      targetNumToDelete = extractNum(message.getText(), pos);
    }
    catch (const std::logic_error &e)
    {
      logAndSendErrorMessage(req, commandName, "Can not perform command. Incorrect number passed");
      return;
    }
    if (!checkIfNumInTargetsTable(targetNumToDelete, req, commandName, "Can not perform command. Given num does not exist"))
      return;
    {
      auto configWrap = ConfigHolder::getReadWriteConfig();
      auto &config = configWrap.config;
      config.targetsTable.removeByNum(targetNumToDelete);
    }
    BOOST_LOG_TRIVIAL(info) << commandName << ": Successfully removed target " << targetNumToDelete;
    req.message("Successfully removed target " + std::to_string(targetNumToDelete)).execute();
    return;
  }
}
