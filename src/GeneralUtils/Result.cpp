#include "Result.hpp"

details_::ErrorResult_t make_error_result(std::string errMsg)
{
  details_::ErrorResult_t err;
  err.errMsg = std::move(errMsg);
  return err;
}

Result<void> make_success_result()
{
  return Result<void>();
}