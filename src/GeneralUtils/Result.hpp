#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

//TODO possibly specialization for Obj_t = void?

template < class Obj_t >
class Result
{
public:
  using ErrorMessage_t = std::string;
  Result();
  Result(Obj_t);
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

template < class Obj_t >
Result< Obj_t > make_error_result(Result< Obj_t >::ErrorMessage_t);


template< class Obj_t >
inline Result< Obj_t > make_error_result(std::string errMsg)
{
  Result< Obj_t > result;
  result.setError(std::move(errMsg));
  return result;
}

#endif
