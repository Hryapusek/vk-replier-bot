#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

//TODO make realization

namespace details_
{
  struct ErrorResult_t {
    using ErrorMessage_t = std::string;
    ErrorMessage_t errMsg;
  };
  using ErrorMessage_t = std::string;
}


template < class Obj_t >
class Result
{
  using ErrorResult_t = details_::ErrorResult_t;
  using ErrorMessage_t = details_::ErrorMessage_t;
public:
  Result(Obj_t);
  Result(ErrorResult_t);
  void setObject(Obj_t &&obj);
  void setObject(const Obj_t &obj);
  void setError(ErrorMessage_t msg);
  const ErrorMessage_t &getErrorMessage();
  bool hasObject();
  Obj_t &getObject();

  operator bool();

private:
  std::optional< Obj_t > obj_;
  std::optional< ErrorMessage_t > errMsg_;
};

template<>
class Result<void>
{
  using ErrorResult_t = details_::ErrorResult_t;
public:
  using ErrorMessage_t = details_::ErrorMessage_t;
  Result();
  Result(ErrorResult_t);
  void setError(ErrorMessage_t msg);
  ErrorMessage_t getErrorMessage();

  operator bool();

private:
  std::optional< ErrorMessage_t > errMsg_;
};

details_::ErrorResult_t make_error_result(std::string errMsg);

template < class Obj_t >
Result< std::remove_reference_t<Obj_t> > make_success_result(Obj_t &&obj);

Result< void > make_success_result();


template <class Obj_t>
inline Result<Obj_t>::Result(Obj_t obj) :
  obj_(obj)
{}

inline Result<void>::Result()
{}

inline Result<void>::Result(ErrorResult_t err) :
  errMsg_(std::move(err.errMsg))
{}

void Result<void>::setError(ErrorMessage_t errMsg)
{
  errMsg_ = std::move(errMsg);
}

details_::ErrorMessage_t Result<void>::getErrorMessage()
{
  return *errMsg_;
}

Result<void>::operator bool()
{
  return !errMsg_.has_value();
}

template <class Obj_t>
inline Result<Obj_t>::Result(ErrorResult_t err) : errMsg_(std::move(err.errMsg))
{}

template <class Obj_t>
inline void Result<Obj_t>::setObject(Obj_t &&obj)
{
  obj_ = std::move(obj);
}

template <class Obj_t>
inline void Result<Obj_t>::setObject(const Obj_t &obj)
{
  obj_ = obj;
}

template <class Obj_t>
inline void Result<Obj_t>::setError(details_::ErrorMessage_t msg)
{
  errMsg_ = std::move(msg);
}

template <class Obj_t>
inline const details_::ErrorMessage_t &Result<Obj_t>::getErrorMessage()
{
  return errMsg_;
}

template <class Obj_t>
inline bool Result<Obj_t>::hasObject()
{
  return obj_.has_value();
}

template <class Obj_t>
inline Obj_t &Result<Obj_t>::getObject()
{
  return *obj_;
}

template <class Obj_t>
inline Result<Obj_t>::operator bool()
{
  return !errMsg_.has_value();
}

template <class Obj_t>
inline Result<std::remove_reference_t<Obj_t>> make_success_result(Obj_t &&obj)
{
  Result<std::remove_reference_t<Obj_t>> res;
  res.setObject(std::forward(obj));
  return res;
}

#endif