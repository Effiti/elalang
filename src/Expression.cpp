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
  return mLhs.isConstEvaluable() && mRhs.isConstEvaluable();
}

const bool Expression::isConstEvaluable() {
    return 0;
}

