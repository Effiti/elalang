#pragma once

#include <utility>
#include <variant>

#include "../BaseType.h"
#include "../Ela.hpp"
#include "Node.h"
namespace Ela::Analysis {
  class Type;
}
namespace Ela::TypeExpressions {
using Ela::BaseType;
using enum Ela::BaseType;

class TypeExpression : public Ela::Node {
 public:
  virtual std::string toString() const { return {"None"}; }
  virtual bool operator!=(const TypeExpression &other) const { return true; }
  virtual bool operator==(const TypeExpression &other) const { return true; }
  // TODO search some sort of typedef-Table for defined classes.
  // virtual llvm::Type *getIRType(Emitter::Emitter &) { return nullptr; };
  virtual bool isInfer() const { return false; }
  virtual Analysis::Type toAnalysisType();
};

class PointerType : public TypeExpression {
 public:
  std::shared_ptr<TypeExpression> base_type;
  PointerType(std::shared_ptr<TypeExpression> base_type)
      : base_type{base_type} {};
  std::string toString() const override {
    return "Ptr[ " + base_type->toString() + " ]";
  }
  // llvm::Type *getIRType(Emitter::Emitter &) override;
  Analysis::Type toAnalysisType() override;
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
      return to_string(get<BaseType>(type));
    }
  }

  // TODO infer should be a seperate struct
  bool isInfer() const override {
    return std::holds_alternative<BaseType>(type) && get<BaseType>(type) == BaseType::Infer;
  }
  // llvm::Type *getIRType(Emitter::Emitter &) override;
  Analysis::Type toAnalysisType() override;
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
  Analysis::Type toAnalysisType() override;
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
  Analysis::Type toAnalysisType() override;
};

}  // namespace Ela::TypeExpressions
