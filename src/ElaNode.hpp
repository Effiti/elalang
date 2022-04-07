#pragma once

#include "Ela.hpp"

enum class NodeType {
  OperatorNode,
  BlockNode,
  InbuiltFunctionNode,
  FunctionCallNode,
  VariableReferenceNode,
  None
};

enum class OperatorType {
  MultiplicationOperator,
  PlusOperator,
  MinusOperator,
  DivisionOperator,
  RightShiftOperator,
  LeftShiftOperator,
};
std::string humanReadableOperatorType(OperatorType type);

enum class PrimitiveLiteralType {
  ElaString,
  ElaInt,
  ElaFloat,
  ElaChar,
};

class ElaNode {
public:
  std::string toString();
  NodeType nodeType;
  bool isConstComputable;
  bool isConstComputabilityDetermined;
};

class OperationNode : public ElaNode {
public:
  OperationNode(OperatorType t) : type{t} {};
  OperatorType type;
  std::vector<ElaNode> operands;
};
class BlockNode : public ElaNode {
public:
  BlockNode(std::vector<ElaNode> subNodes) : nodes{subNodes} {};
  std::vector<ElaNode> nodes;
};
class FunctionCallNode : public ElaNode {
public:
  FunctionCallNode(std::string_view f) : functionName{f} {};
  std::string functionName;
  std::vector<ElaNode> arguments;

};
class VariableReferenceNode : public ElaNode {
public:
  VariableReferenceNode(std::string v) : variableName{v} {};

  std::string variableName;
};

// example: var int a
class VariableDeclarationNode : public ElaNode {

};
// example: a = 10;
class VariableDefinitionNode : public ElaNode {
public:
  VariableDefinitionNode(std::string v) : variableName{v} {};
  std::string variableName;
  ElaNode value;
};
