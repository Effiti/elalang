#pragma once

#include "Ela.hpp"

enum class NodeType {

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
};

class OperatorNode : public ElaNode {
public:
  OperatorNode(OperatorType t) : type{t} {};

  OperatorType type;
};
class BlockNode : public ElaNode {
public:
  BlockNode(std::vector<ElaNode> subNodes) : nodes{subNodes} {};
  std::vector<ElaNode> nodes;
};
class InbuiltFunctionNode : public ElaNode {
public:
  InbuiltFunctionNode(std::string_view f) : functionName{f} {};
  std::string_view functionName;
};
class VariableNode : public ElaNode {
public:
  VariableNode(std::string v) : variableName{v} {};

  std::string variableName;
};
class VariableDefinitionNode : public ElaNode {
public:
  VariableDefinitionNode(std::string v) : variableName{v} {};

  std::string variableName;
};
class VariableAssingNode : public ElaNode {};
class ConstNode : public ElaNode {};
class ConstLiteralNode : public ElaNode {
  PrimitiveLiteralType type;
};
