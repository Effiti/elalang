#pragma once

#include <utility>

#include "../Ela.hpp"
#include "Node.h"
#include "TypeExpression.h"

enum class BinaryOperatorType {
  None,
  Multiplication,
  Plus,
  Minus,
  Division,
  RightShift,
  LeftShift,
  Equal,

  MemberAccess,
};

enum class UnaryOperatorType {
  None,
  Plus,
  Minus,
  PreIncrement,
  PostIncrement,
  PreDecrement,
  PostDecrement,
};
namespace Ela::Analysis {
class ExpressionVisitor;
}
namespace Ela::Emitter {
  class Emitter;
}
namespace Ela::Expressions {
class Expression : public Node {
 public:
  virtual std::size_t getType(Analysis::ExpressionVisitor& c) const {
    return -1;
  };
  virtual llvm::Value *codegen(Emitter::Emitter& e) {return nullptr;}
  virtual std::string toString();
};

class Unary : public Expression {
 public:
  Unary(std::shared_ptr<Expression> p, UnaryOperatorType type)
      : expression{std::move(p)}, op{type} {};
  std::shared_ptr<Expression> expression;
  UnaryOperatorType op;

  std::string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
  llvm::Value *codegen(Emitter::Emitter&) override;
};

class Binary : public Expression {
 public:
  Binary(std::shared_ptr<Expression> left, BinaryOperatorType operatorType,
         std::shared_ptr<Expression> right)
      : lhs{std::move(left)}, rhs{std::move(right)}, op{operatorType} {};

  std::shared_ptr<Expression> lhs;
  std::shared_ptr<Expression> rhs;
  const BinaryOperatorType op;

  std::string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
};

class Primary : public Expression {};

class Parenthed : public Primary {
 public:
  explicit Parenthed(std::shared_ptr<Expression> pSubExpr)
      : subExpr{std::move(pSubExpr)} {};

  std::string toString() override;

  std::shared_ptr<Expression> subExpr;
};

class IntegerLiteral : public Primary {
 public:
  IntegerLiteral(int v) : value{v} {};

  const int value;

  std::string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;

  llvm::Value *codegen(Emitter::Emitter&) override;
};

class NullExpression : public Primary {
 public:
  std::string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
};

class FunctionCall : public Primary {
 public:
  FunctionCall(std::string name, vector<std::shared_ptr<Expression>> params)
      : callParams{std::move(params)}, functionName{std::move(name)} {}

  const string functionName;
  const vector<std::shared_ptr<Expression>> callParams;

  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
  string toString() override;
  llvm::Value* codegen(Emitter::Emitter &e) override;
};
class ArrayLiteral : public Primary {
 public:
  ArrayLiteral(vector<std::shared_ptr<Expression>> elements)
      : elements(elements){};
  const vector<std::shared_ptr<Expression>> elements;
  string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
};

class VariableReference : public Primary {
 public:
  VariableReference(std::string name) : variableName{std::move(name)} {};
  const std::string variableName;

  string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
  llvm::Value *codegen(Emitter::Emitter&e ) override;
};

class StringLiteral : public Primary {
 public:
  StringLiteral(std::string literal) : value(std::move(literal)){};
  const std::string value;

  std::string toString() override;

  std::string unescaped();
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
};

class BooleanLiteral : public Primary {
 public:
  explicit BooleanLiteral(bool literal) : value{literal} {};
  bool value;

  std::string toString() override;
  std::size_t getType(Analysis::ExpressionVisitor& c) const override;
};

}  // namespace Ela::Expressions
