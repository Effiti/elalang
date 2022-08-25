#pragma once
#include "Expression.hpp"

class ImportStatement : public Node {
public:
  std::string_view module;
};
class Statement : public Node {};
class Parameter : public Node {
  TypeExpressions::TypeExpression type;
  std::string_view name;
};
class FunctionDefinition : public Statement {
public:
  TypeExpressions::TypeExpression returnType;
  std::string_view functionName;
  SameTypeNodeList<Parameter> parameters;
};
class Programm : public Node {
public:
  SameTypeNodeList<ImportStatement> importStatements;
  SameTypeNodeList<FunctionDefinition> functionDefinitions;
};
