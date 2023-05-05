#pragma once

#include <iterator>

#include "../parser/Statement.h"
#include "./SymbolTable.h"

namespace Ela::Analysis {
class ProgramVisitor;

class ExpressionVisitor {
 private:
  const VariableSymbolTable& variables;
  TypeTable& types;

 public:
  ExpressionVisitor(const VariableSymbolTable& variables, TypeTable& types)
      : variables(variables), types(types){};
  void visitBinaryExpression(const Expressions::Binary& binary);
  void visitUnaryExpression(const Expressions::Unary& unary);
  std::size_t getVariableType(const std::string& name);
  std::size_t getArrayType(const std::size_t baseType);
};

class TypeExpressionVisitor {
 public:
};
class StatementVisitor {
 private:
  unsigned int nesting;

  VariableSymbolTable variables;
  ExpressionVisitor expressionVisitor;
  TypeTable typeTable;

 public:
  StatementVisitor()
      : nesting{0},
        variables{},
        typeTable{},
        expressionVisitor(variables, typeTable) {}
  void visitBlock(Ela::Statements::BlockStatement const& s);
  void visitVariableDefinition(
      const Ela::Statements::VariableDefinitionStatement& s);
  void print() {
    std::cout << "types:" << std::endl;
    typeTable.print();
    std::cout << "---------" << std::endl << "vars:" << std::endl;
    variables.print();
  }

  friend Analysis::ProgramVisitor;
  friend Statements::ExpressionStatement;
  friend Statements::IfStatement;
};
class ProgramVisitor {
 private:
  const Statements::Program program;
  StatementVisitor v;

 public:
  ProgramVisitor(const Statements::Program& p) : program{p} {}
  void check();
  void compile();
};

}  // namespace Ela::Analysis
