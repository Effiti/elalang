#pragma once

#include <utility>
#include <variant>

#include "../Ela.hpp"
#include "Node.h"

namespace Ela::TypeExpressions {
enum BaseType {
  Integer,
  String,
  Char,
  Float,
  Double,
  Pointer,
  Boolean,
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

static std::string to_string(BaseType type) {
  switch (type) {
    case Integer:
      return "Integer";
    case String:
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
  }
}



class TypeExpression : public Ela::Node {
 public:
  virtual std::string toString() const { return {"None"}; }
  virtual bool operator!=(const TypeExpression &other) const { return true; }
  virtual bool operator==(const TypeExpression &other) const { return true; }
  // TODO search some sort of typedef-Table for defined classes.
  virtual llvm::Type *getIRType(Emitter::Emitter&) { return nullptr; };
};

class SimpleType : public TypeExpression {
 public:
  explicit SimpleType(const std::variant<const std::string, BaseType> &name)
      : type{name} {};
  std::variant<const std::string, BaseType> type;

  std::string toString() const override {
    if (std::holds_alternative<const std::string>(type)) {
      return get<const std::string>(type);
    } else {
      return TypeExpressions::to_string(get<BaseType>(type));
    }
  }
  llvm::Type* getIRType(Emitter::Emitter&) override;
};

class TupleTypeExpression : public TypeExpression {
 public:
  vector<std::shared_ptr<TypeExpression>> types;
  TupleTypeExpression(vector<std::shared_ptr<TypeExpression>> types)
      : types(types) {}
  std::string toString() const override {
    std::string tuple_args;
    for (const auto &item : types) {
      std::string str;
      str = item->toString();
      tuple_args += str + ", ";
    }
    return "(" + tuple_args + ")";
  }
};

class TypeTemplateExpression : public TypeExpression {
 public:
  SimpleType templatedType;
  vector<std::variant<std::shared_ptr<TypeExpression>, int>> templateArguments;

  TypeTemplateExpression(
      SimpleType baseType,
      std::vector<std::variant<std::shared_ptr<TypeExpression>, int>>
          templateArguments)
      : templateArguments(std::move(templateArguments)),
        templatedType(std::move(baseType)) {}

  std::string toString() const override {
    std::string template_args;
    for (const auto &item : templateArguments) {
      std::string str;
      if (std::holds_alternative<int>(item))
        str = std::to_string(std::get<int>(item));
      else
        str = std::get<std::shared_ptr<TypeExpression>>(item)->toString();
      template_args += str + ", ";
    }
    return templatedType.toString() + "[" + template_args + "]";
  }
};

static std::optional<BaseType> getBaseType(const std::string &type) {
  // we do not check for the void-type as it does not occur in identifiers:
  // void-type functions do not have an explicitly annotated return type.
  if (type == "int") return Integer;
  if (type == "string") return String;
  if (type == "pointer") return Pointer;
  if (type == "char") return Char;
  if (type == "float") return Float;
  if (type == "double") return Double;
  if (type == "bool") return Boolean;
  if (type == "array")
    return Array;
  else
    return std::nullopt;
}

}  // namespace Ela::TypeExpressions
