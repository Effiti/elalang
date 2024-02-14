#pragma once

#include <iterator>

#include "../parser/Statement.h"
#include "./SymbolTable.h"

namespace Ela::Analysis {
class ProgramVisitor;

class ExpressionVisitor {
 private:


 public:
  const VariableSymbolTable& variables;
  const FunctionSymbolTable& functions;
  TypeTable& types;
  ExpressionVisitor(const VariableSymbolTable& variables, const FunctionSymbolTable& functions, TypeTable& types)
      : variables(variables),  functions{functions}, types(types){};
  void visitBinaryExpression(const Expressions::Binary& binary);
  void visitUnaryExpression(const Expressions::Unary& unary);
  std::size_t getVariableType(const std::string& name);
  std::size_t getArrayType(const std::size_t baseType);
};
class FunctionParameter {
  public:
   const std::string name;
   const std::size_t type;
   FunctionParameter(const std::string& name, const std::size_t& type) : name(std::move(name)), type(std::move(type)) {}
};
// TODO should contain argTypes and names for recursion => store args here and not in Vartable
class IncompleteFunction {
  public:
   std::size_t returnType;
   std::string name;
   std::vector<FunctionParameter> args;
   IncompleteFunction(std::string name, std::size_t returnType, std::vector<FunctionParameter> args) : name{name}, returnType{returnType}, args{args} {};
 };

class TypeExpressionVisitor {
 public:
};
class StatementVisitor {
 private:
  unsigned int nesting;

  VariableSymbolTable variables;
  FunctionSymbolTable functions;
  ExpressionVisitor expressionVisitor;
  TypeTable typeTable;
  IncompleteFunction contextFn;
 public:
  StatementVisitor()
      : nesting{0},
        variables{},
        typeTable{},
        functions{},
        // very Hacky, but there are no top-level-statements, so return statements at the top level dont exist
        contextFn{"", typeTable.getBaseTypeId(TypeExpressions::Void), std::vector<FunctionParameter>{}},
        expressionVisitor(variables, functions, typeTable) {}
  void visitBlock(Ela::Statements::BlockStatement const& s, bool fnBlock);
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
  friend Statements::ReturnStatement;
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
