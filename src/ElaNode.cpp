#include "ElaNode.hpp"
#include "Ela.hpp"
#include <fmt/core.h>
std::string humanReadableNodeType(NodeType type) { return "Unimplemented"; }
std::string ElaNode::toString() {
  return fmt::format("Node of Type {0}", humanReadableNodeType(nodeType));
}
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
