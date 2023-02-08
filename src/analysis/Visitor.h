#pragma once

#include "../parser/Statement.h"
#include "./SymbolTable.h"

namespace Ela::Analysis {

class ExpressionVisitor {};
class ProgramVisitor {
  private:
    const Statements::Program program;
 public:
    ProgramVisitor(const Statements::Program & p ) : program{p} {}
    void check();
    void compile();

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
