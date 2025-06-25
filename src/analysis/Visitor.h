#pragma once

#include <iterator>
#include <memory>

#include "../parser/Statement.h"
#include "./SymbolTable.h"
#include "Type.h"

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
  // void visitBinaryExpression(const Expressions::Binary& binary);
  // void visitUnaryExpression(const Expressions::Unary& unary);
  Type getArrayLiteralType(const Expressions::ArrayLiteral);
  std::shared_ptr<Type> getVariableTypeAn(const std::string& name) const;
  std::shared_ptr<Type> getBinaryType(const Type &lhs, const Type &rhs, const BinaryOperatorType &op) const;
  std::shared_ptr<Type> getUnaryType(const Type &base, const UnaryOperatorType &op) const;
  Type getFunctionCallType(const Expressions::FunctionCall);
  std::size_t getVariableType(const std::string& name);
  std::size_t getArrayType(const std::size_t baseType);
  std::size_t getPointerType(const std::size_t baseType);
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
   std::vector<std::shared_ptr<VariableDefinitionSymbol>> vars{};
   IncompleteFunction(std::string name, std::size_t returnType,
                      std::vector<FunctionParameter> args)
       : name{name}, returnType{returnType}, args{args} {};
 };

class TypeExpressionVisitor {
 public:
};
class StatementVisitor {
 public:
  unsigned int nesting;

  VariableSymbolTable variables;
  FunctionSymbolTable functions;
  ExpressionVisitor expressionVisitor;
  TypeTable typeTable;
  IncompleteFunction contextFn;
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
      Ela::Statements::VariableDefinitionStatement& s);
  void print() {
    std::cout << "types:" << std::endl;
    typeTable.print();
    std::cout << "---------" << std::endl << "vars:" << std::endl;
    variables.print();
  }

};
class ProgramVisitor {
 private:
  Statements::Program program;
  StatementVisitor v;

 public:
  ProgramVisitor(const Statements::Program& p) : program{p} {}
  void check();
  void compile();
  void print() {
   v.print();
   for (const auto& def : program.functionDefinitions) {
    for(const auto& v : def.decls)
      std::cout << v->name << " : " << v->type << " = "
                << v->initialValue->toString() << std::endl;
   }
  }
};

}  // namespace Ela::Analysis
