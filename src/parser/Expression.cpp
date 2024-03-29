#include "Expression.h"

#include <memory>

#include "../Ela.hpp"
namespace Ela::Expressions {
using namespace Expressions;

std::string humanReadableBinaryOperatorType(BinaryOperatorType type) {
  switch (type) {
    case BinaryOperatorType::Multiplication:
      return "Mul";
    case BinaryOperatorType::Plus:
      return "Add";
    case BinaryOperatorType::Minus:
      return "Sub";
    case BinaryOperatorType::Division:
      return "Div";
    case BinaryOperatorType::RightShift:
      return "Rshift";
    case BinaryOperatorType::LeftShift:
      return "Lshift";
    case BinaryOperatorType::Equal:
      return "Eq";
    case BinaryOperatorType::MemberAccess:
      return "Member";
    case BinaryOperatorType::UnEqual:
      return "Neq";
    case BinaryOperatorType::Greater:
      return "Greater";
    case BinaryOperatorType::GreaterEqual:
      return "GreaterEq";
    case BinaryOperatorType::Less:
      return "Less";
    case BinaryOperatorType::LessEqual:
      return "LessEq";
    default:
      return "";
  }
}

std::string humanReadableUnaryOperatorType(UnaryOperatorType type) {
  switch (type) {
    case UnaryOperatorType::None:
      return "None";
    case UnaryOperatorType::Plus:
      return "Plus";
    case UnaryOperatorType::Minus:
      return "Minus";
    case UnaryOperatorType::PreIncrement:
      return "PreIncrement";
    case UnaryOperatorType::PostIncrement:
      return "PostIncrement";
    case UnaryOperatorType::PreDecrement:
      return "PreDecrement";
    case UnaryOperatorType::PostDecrement:
      return "PostDecrement";
    case UnaryOperatorType::Not:
      return "Not";
  }
}

std::string Binary::toString() {
  return "[" + lhs->toString() + " " + humanReadableBinaryOperatorType(op) +
         " " + rhs->toString() + "]";
}

std::string Expression::toString() { return {}; }

std::string IntegerLiteral::toString() { return std::to_string(value); }

std::string Parenthed::toString() { return subExpr->toString(); }

std::string NullExpression::toString() { return "null"; }
std::string ArrayLiteral::toString() {
  std::string str;
  for(const auto& element : elements) {
    str += element->toString() + ",";
  }
  return "[" + str + "]";
}

std::string VariableReference::toString() {
  return "var(" + variableName + ")";
}

std::string FunctionCall::toString() {
  std::string params;
  for (const std::shared_ptr<Expression>& param : callParams) {
    params += param->toString();
  }
  if (params.length() != 0) params = params.substr(0, params.length() - 2);
  return "[" + functionName + "(" + params + "))" + "]";
}

std::string StringLiteral::toString() { return "\"" + value + "\""; }

std::string StringLiteral::unescaped() {
  std::string newValue = value;
  for (int i = 0; i < value.length(); ++i) {
    if (value[i] == '\\') {
      if (++i >= value.length()) {
        // the case in which the last character of a string is a single '\'
        // should be stopped by the lexer!
        throw std::runtime_error("impossible state!");
      }
      if (value[i] == 'n') {
        newValue = newValue.substr(0, i - 1) + '\n' +
                   value.substr(i + 1, value.length());
      }
    }
  }
  return newValue;
}

std::string BooleanLiteral::toString() { return value ? "true" : "false"; }

std::string Unary::toString() {
  return "[" + humanReadableUnaryOperatorType(op) + " " +
         expression->toString() + "]";
}
};  // namespace Ela::Expressions
