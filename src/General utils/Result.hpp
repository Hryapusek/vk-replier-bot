#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

template < class Obj_t >
class Result
{
public:
  using ErrorMessage_t = std::string;
  Result();
  void setObject(Obj_t &&object);
  void setObject(const Obj_t &object);
  void setErrorStatus();
  void setErrorMessage();
  ErrorMessage_t getErrorMessage();
  bool hasObject();
  Obj_t &getObject();
  
  operator bool();

private:
  std::optional<Obj_t> obj_;
  bool isError_;
  ErrorMessage_t errorMessage_;
};

#endif
