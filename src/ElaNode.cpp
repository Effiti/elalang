#include "ElaNode.hpp"
#include "Ela.hpp"

std::string ElaNode::toString() { return "empty Node"; }
std::string humanReadableOperatorType(OperatorType type) {
  switch (type) {
  case OperatorType::MultiplicationOperator:
    return "MultiplicationOperator";
  case OperatorType::PlusOperator:
    return "AdditionOperator";
  case OperatorType::MinusOperator:
    return "SubtractionOperator";
  case OperatorType::DivisionOperator:
    return "DivisionOperator";
  case OperatorType::RightShiftOperator:
    return "RightShiftOperator";
  case OperatorType::LeftShiftOperator:
    return "LeftShiftOperator";
  default:
    return "";
  }
}
