#include <stdexcept>

#include "Ela.hpp"

namespace Ela {
enum BaseType {
  Integer,
  CString,
  Char,
  Float,
  Double,
  Boolean,
  /////////////
  Pointer,
  Array,
  Function,
  //"Special" Types
  // Type of functions with no return value
  Void,
  // Type of defined variables wihout type specification (can also be used for
  // other things)
  Infer,
  // Type of the "null" literal
  Null
};

constexpr BaseType FundamentalTypes[] = {
    Integer, CString, Char, Float, Double, Boolean, Void
};

inline bool isFundamental(BaseType type) {
  for (auto bt : FundamentalTypes)
    if (bt == type) return true;
  return false;
}

struct FundamentalType {
  BaseType bt;

  explicit FundamentalType(BaseType type) : bt(type) {
    if (!isFundamental(type))
      throw std::invalid_argument("not a fundamental type");
  }

  operator BaseType() { return bt; }
  bool operator==(const FundamentalType& other) const { return bt == other.bt; }
};

static std::string to_string(BaseType type) {
  switch (type) {
    case Integer:
      return "Integer";
    case CString:
      return "String";
    case Char:
      return "Char";
    case Float:
      return "Float";
    case Double:
      return "Double";
    case Pointer:
      return "Pointer";
    case Boolean:
      return "Boolean";
    case Void:
      return "Void";
    case Array:
      return "Array";
    case Infer:
      return "Infer";
    case Null:
      return "Null";
    case Function:
      return "Function";
    default:
      return "Unimplemented";
  }
}

static std::optional<BaseType> getBaseType(const std::string& type) {
  // we do not check for the void-type as it does not occur in identifiers:
  // void-type functions do not have an explicitly annotated return type.
  if (type == "int") return Integer;
  if (type == "cstring") return CString;
  if (type == "char") return Char;
  if (type == "float") return Float;
  if (type == "double") return Double;
  if (type == "bool") return Boolean;
  if (type == "array")
    return Array;
  else
    return std::nullopt;
}

};  // namespace Ela
