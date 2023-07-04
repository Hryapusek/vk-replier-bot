#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <jsoncpp/json/json.h>
#include <string_view>
#include <string>

/// @throws Json::Exception.
Json::Value stringToJson(std::string_view s);

/**
 * @brief Use this struct to check if field is some of primitive types.
 * @tparam T primitive type.
 */
template < class T >
struct JsonFieldT;

/// @brief Use this struct to check if field is some of Json::ValueType.
struct JsonFieldVT;

/**
 * @param root json in which we search
 * @param field field to find in json
 * @return True if field with same name and type found. False otherwise.
 * @throw Json::Logic_error if root is not null or is not object
 * @overload
 */
template < class T >
bool isCorrectField(const Json::Value &root, const JsonFieldT< T > &field);

/**
 * @param root json in which we search
 * @param field field to find in json
 * @return True if field with same name and type found. False otherwise.
 * @throw Json::Logic_error if root is not null or is not object
 * @overload
 */
bool isCorrectField(const Json::Value &root, const JsonFieldVT &field);

/**
 * @brief Checks if json contains exact field.
 * @throw Json::Exception if does not contain field or it has wrong type.
 * @overload
 */
template < class Field >
void checkJsonField(const Json::Value &root, const Field &field);

/**
 * @brief Checks json fields.
 * @tparam Field one of \b JsonFieldT or \b JsonFieldVT
 * @tparam ...Args empty or one of \b JsonFieldT or \b JsonFieldVT
 * @param root json to check
 * @param field field to find in json
 * @param ...args fields to find in json(can be empty)
 */
template < class ... Args >
void checkJsonFields(const Json::Value &root, const std::tuple< Args... > &fieldsTuple);

namespace
{
  template < size_t I, class ... Args >
  void checkTupleFields(const Json::Value &root, const std::tuple< Args... > &tuple)
  {
    checkJsonField(root, std::get< I >(tuple));
    if constexpr (I + 1 < sizeof...(Args))
      checkTupleFields< I + 1 >(root, tuple);
  }
}

template < class T >
struct JsonFieldT
{
  using type = T;

  JsonFieldT(std::string fieldName, bool isNecessary = true) :
    fieldName(std::move(fieldName)),
    isNecessary(isNecessary)
  { }

  const std::string &getName() const
  {
    return fieldName;
  }

  bool isFieldNecessary() const
  {
    return this->isNecessary;
  }

private:
  std::string fieldName;
  bool isNecessary;
};

struct JsonFieldVT
{
  JsonFieldVT(std::string fieldName, Json::ValueType valueType, bool isNecessary = true) :
    fieldName(std::move(fieldName)),
    valueType(valueType),
    isNecessary(isNecessary)
  { }

  Json::ValueType getValueType() const
  {
    return valueType;
  }

  const std::string &getName() const
  {
    return fieldName;
  }

  bool isFieldNecessary() const
  {
    return this->isNecessary;
  }

private:
  std::string fieldName;
  Json::ValueType valueType;
  bool isNecessary;
};

template < class T >
bool isCorrectField(const Json::Value &root, const JsonFieldT< T > &field)
{
  bool hasField = root.isMember(field.getName());
  if (!hasField && !field.isFieldNecessary())
    return true;
  bool correctType = root[field.getName()].template is< typename std::remove_reference_t< T > >();
  return hasField && correctType;
}

bool isCorrectField(const Json::Value &root, const JsonFieldVT &field)
{
  bool hasField = root.isMember(field.getName());
  if (!hasField && !field.isFieldNecessary())
    return true;
  bool correctType = root[field.getName()].type() == field.getValueType();
  return hasField && correctType;
}

template < class Field >
void checkJsonField(const Json::Value &root, const Field &field)
{
  if (!isCorrectField(root, field))
    throw Json::Exception(field.getName() + " - field was not found");
}

template < class ... Args >
void checkJsonFields(const Json::Value &root, const std::tuple< Args... > &fieldsTuple)
{
  if constexpr (sizeof...(Args) != 0)
    checkTupleFields< 0 >(root, fieldsTuple);
}

#endif
