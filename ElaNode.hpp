#pragma once

#include "Ela.hpp"
#include <cstdint>

enum class OperatorType {
  MultiplicationOperator,
  PlusOperator,
  MinusOperator,
  DivisionOperator,
};

enum class InbuiltFunctionType {
  VarDecl,

};

class ElaNode {};

class OperatorNode: public ElaNode {
  OperatorType type;
};
class SubStackNode : public ElaNode {
  vector<ElaNode> subnodes;
};
class InbuiltFunctionNode : public ElaNode {
  std::string_view functionName;
};
class CorpseRemoveNode : public ElaNode {};
class VariableNode : public ElaNode {
  std::string variableName;
};
class PipeNode {};
class ExecQualifier {};
