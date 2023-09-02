#include "CommandsPool.hpp"
#include "Commands/Commands.hpp"

namespace msg_proc::commands
{
  const std::vector< std::shared_ptr< CommandI > > CommandsPool::vec = {
    std::make_shared< ChangeModeCommand >(std::vector<std::string>{"/change_mode", "/cm"}),
    std::make_shared< DelSourceCommand >(std::vector<std::string>{"/del_source", "/ds"}),
    std::make_shared< DelTargetByNumCommand >(std::vector<std::string>{"/del_target_by_num", "/dtbn"}),
    std::make_shared< DelTargetCommand >(std::vector<std::string>{"/del_target", "/dt"}),
    std::make_shared< RegGodlikeCommand >(std::vector<std::string>{"/reg_godlike", "/rg"}),
    std::make_shared< RegSourceCommand >(std::vector<std::string>{"/reg_source", "/rs"}),
    std::make_shared< RegTargetCommand >(std::vector<std::string>{"/reg_target", "/rt"}),
    std::make_shared< StatusCommand >(std::vector<std::string>{"/status", "/s"}),
  };
}
