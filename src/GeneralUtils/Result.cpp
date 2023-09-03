#include "Result.hpp"

details_::ErrorResult_t make_error_result(std::string errMsg)
{
  details_::ErrorResult_t err;
  err.errMsg = std::move(errMsg);
  return err;
}

Result< void > make_success_result()
{
  return Result< void >();
}

Result< void >::Result()
{ }

Result< void >::Result(ErrorResult_t err) :
  errMsg_(std::move(err.errMsg))
{ }

void Result< void >::setError(ErrorMessage_t errMsg)
{
  errMsg_ = std::move(errMsg);
}

const details_::ErrorMessage_t &Result< void >::getErrorMessage()
{
  return *errMsg_;
}

Result< void >::operator bool()
{
  return !errMsg_.has_value();
}