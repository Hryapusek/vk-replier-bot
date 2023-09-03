#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <jsoncpp/json/json.h>
#include <string_view>
#include <string>
#include <functional>

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
    fieldName(fieldName),
    isNecessary(isNecessary)
  {
    funcIfIncorrect = [fieldName=this->fieldName]() {defaultFuncIfIncorrect(fieldName);};
  }

  /// @param fieldName Expected name of the field
  /// @param isNecessary True if field must present in json
  /// @param funcIfIncorrect Function that is called if json
  /// will fail field validation
  JsonFieldT(std::string fieldName, bool isNecessary, std::function< void() > funcIfIncorrect) :
    fieldName(fieldName),
    isNecessary(isNecessary),
    funcIfIncorrect(funcIfIncorrect)
  { }

  /// @brief Checks if valid field respectively to this present in json.
  /// @throw Json::Exception if field is not valid and no other function
  /// was passed in constructor
  void validateJson(const Json::Value &root) const
  {
    if ((!root.isNull() && !root.isObject()) || !isValid(root))
      if (funcIfIncorrect)
        funcIfIncorrect();
  }

  /// @brief Find out if valid field respectively to this present in json. 
  /// @return True or False depend on isNecessary and Type conditions.
  bool isValid(const Json::Value &root) const
  {
    bool hasField = root.isMember(fieldName);
    if (!hasField && !isNecessary)
      return true;
    bool correctType = root[fieldName].template is< typename std::remove_reference_t< T > >();
    return hasField && correctType;
  }

  static void defaultFuncIfIncorrect(std::string field)
  {
    throw Json::Exception(field + " - field was not found or it's type incorrect");
  }

private:
  std::string fieldName;
  bool isNecessary;
  std::function< void() > funcIfIncorrect;
};

struct JsonFieldVT
{
  JsonFieldVT(std::string fieldName, Json::ValueType valueType, bool isNecessary = true) :
    fieldName(fieldName),
    valueType(valueType),
    isNecessary(isNecessary)
  {
    funcIfIncorrect = [fieldName=this->fieldName]() {defaultFuncIfIncorrect(fieldName);};
  }

  JsonFieldVT(std::string fieldName, Json::ValueType valueType, bool isNecessary, std::function< void() > funcIfIncorrect) :
    fieldName(fieldName),
    valueType(valueType),
    isNecessary(isNecessary),
    funcIfIncorrect(funcIfIncorrect)
  { }

  /// @brief Checks if valid field respectively to this present in json.
  /// @throw Json::Exception if field is not valid and no other function
  /// was passed in constructor
  void validateJson(const Json::Value &root) const
  {
    if ((!root.isNull() && !root.isObject()) || !isValid(root))
      if (funcIfIncorrect)
        funcIfIncorrect();
  }

  /// @brief Find out if valid field respectively to this present in json. 
  /// @return True or False depend on isNecessary and Type conditions.
  bool isValid(const Json::Value &root) const
  {
    bool hasField = root.isMember(fieldName);
    if (!hasField && !isNecessary)
      return true;
    bool correctType = root[fieldName].type() == valueType;
    return hasField && correctType;
  }

  static void defaultFuncIfIncorrect(std::string field)
  {
    throw Json::Exception(field + " - field was not found or it's type incorrect");
  }

private:
  std::string fieldName;
  Json::ValueType valueType;
  bool isNecessary;
  std::function< void() > funcIfIncorrect;
};

template < class Field >
void checkJsonField(const Json::Value &root, const Field &field)
{
  field.validateJson(root);
}

template < class ... Args >
void checkJsonFields(const Json::Value &root, const std::tuple< Args... > &fieldsTuple)
{
  if constexpr (sizeof...(Args) != 0)
    checkTupleFields< 0 >(root, fieldsTuple);
}

#endif
