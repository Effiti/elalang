#pragma once
#include "Ela.hpp"
#include "Node.hpp"
enum class OperatorType {
  Multiplication,
  Plus,
  Minus,
  Division,
  RightShift,
  LeftShift

};
namespace TypeExpressions {
  class TypeExpression : public NonLeafNode {
  

  };
  class PrimaryTypeExpression : public TypeExpression {

  };
  class TypeVariable : public PrimaryTypeExpression {

  };
}


namespace Expressions {



class Expression : public Node {
public:
  const virtual bool isConstEvaluable();
};
class Binary : public Expression {
private:
  Expression mLhs;
  Expression mRhs;
  OperatorType mOp;

public:
  Binary(Expression rhs, OperatorType op, Expression lhs)
      : mLhs{lhs}, mRhs{rhs}, mOp{op} {};
  const bool isConstEvaluable();
};

class PrimaryExpression : public Expression {};

class Parenthed : public PrimaryExpression {
public:
  Parenthed(Expression SubExpr);
  Expression SubExpr;
};
class IntegerLiteral : public PrimaryExpression {
public:
  int value;
  const bool isConstEvaluable() { return true; }
};
} // namespace Expressions
