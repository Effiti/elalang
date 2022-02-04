#pragma once

#include "Ela.hpp"
#include <cstdint>

enum class OperatorType {
  MultiplicationOperator,
  PlusOperator,
  MinusOperator,
  DivisionOperator,
};

class ElaNode {};

class OperationNode : public ElaNode {
  OperatorType type;
};
class SubStackNode : public ElaNode {
  bool execute;
  bool constant;
  vector<ElaNode> subnodes;
};
class FunctionNode : public ElaNode {
  std::string_view functionName;
};
class CorpseRemoveNode : public ElaNode {};
