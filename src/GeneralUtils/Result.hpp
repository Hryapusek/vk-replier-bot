#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

//TODO possibly specialization for Obj_t = void?

namespace details_
{
  struct ErrorResult_t {
    using ErrorMessage_t = std::string;
    ErrorMessage_t errMsg;
  };
}

template < class Obj_t >
class Result
{
  using ErrorResult_t = details_::ErrorResult_t;
public:
  using ErrorMessage_t = std::string;
  Result(Obj_t);
  Result(ErrorResult_t);
  void setObject(Obj_t &&object);
  void setObject(const Obj_t &object);
  void setError(ErrorMessage_t msg);
  ErrorMessage_t getErrorMessage();
  bool hasObject();
  Obj_t &getObject();

  operator bool();

private:
  std::optional< Obj_t > obj_;
  std::optional< ErrorMessage_t > errorMessage_;
};

template<>
class Result<void>
{
  using ErrorResult_t = details_::ErrorResult_t;
public:
  using ErrorMessage_t = std::string;
  Result();
  Result(ErrorResult_t);
  void setError(ErrorMessage_t msg);
  ErrorMessage_t getErrorMessage();

  operator bool();

private:
  std::optional< ErrorMessage_t > errorMessage_;
};

details_::ErrorResult_t make_error_result(std::string errMsg);

template < class Obj_t >
Result< Obj_t > make_success_result(Obj_t &&obj);

Result< void > make_success_result();

#endif
