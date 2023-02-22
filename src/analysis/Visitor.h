#pragma once

#include "../parser/Statement.h"
#include "./SymbolTable.h"

namespace Ela::Analysis {

class ExpressionVisitor {
 private:
  const VariableSymbolTable& variables;

 public:
  ExpressionVisitor(const VariableSymbolTable& variables_)
      : variables(variables_){};
  void visitBinaryExpression(const Expressions::Binary& binary);
  void visitUnaryExpression(const Expressions::Unary& unary);
};
class ProgramVisitor {
 private:
  const Statements::Program program;

 public:
  ProgramVisitor(const Statements::Program& p) : program{p} {}
  void check();
  void compile();
};

class TypeExpressionVisitor {
 public:
};
class StatementVisitor {
 private:
  uint nesting;

  VariableSymbolTable variables;
  ExpressionVisitor expressionVisitor;

 public:
  void visitBlock(Ela::Statements::BlockStatement const& s);
  void visitVariableDefinition(
      const Ela::Statements::VariableDefinitionStatement& s);
};
}  // namespace Ela::Analysis
