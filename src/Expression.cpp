#include "Expression.h"
#include "Ela.hpp"
#include <fmt/core.h>
using namespace Expressions;

std::string humanReadableOperatorType(OperatorType type) {
  switch (type) {
  case OperatorType::Multiplication:
    return "Asterisk";
  case OperatorType::Plus:
    return "AdditionOperator";
  case OperatorType::Minus:
    return "SubtractionOperator";
  case OperatorType::Division:
    return "DivisionOperator";
  case OperatorType::RightShift:
    return "RightShiftOperator";
  case OperatorType::LeftShift:
    return "LeftShiftOperator";
  default:
    return "";
  }
}

const bool Binary::isConstEvaluable() {
  return rhs->isConstEvaluable() && rhs->isConstEvaluable();
}

const bool Expression::isConstEvaluable() {
    throw new std::runtime_error("Expression does not implement ");
    return false;
}

std::string Expression::toString() {
    return {};
}

std::string IntegerLiteral::toString() {
   return std::to_string(value);
}

std::string Parenthed::toString() {
   return subExpr->toString();
}
