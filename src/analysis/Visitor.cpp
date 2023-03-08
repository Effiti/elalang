#include "Visitor.h"

namespace Ela {
namespace Analysis {
using std::unique_ptr;
void StatementVisitor::visitVariableDefinition(
    const Statements::VariableDefinitionStatement& s) {
  if (variables.hasSymbol(s.name))
    std::cerr << colors["yellow"] << "WARN: overriding local variable"
              << colors["end"] << std::endl;

  // TODO: Type inference
  int typeId = typeTable.getType(s.type->toString());
  if (typeId == -1) {
    typeId = typeTable.add(TypeEntry(s.type->toString(), s.type));
  }
  int exprTypeId = s.value->getType(expressionVisitor);
  // if the types are not equal, there are two possibilities
  if (exprTypeId != typeId) {
    // if the type is supposed to be inferred, *only* the lhs type is Void.
    if (typeId == typeTable.getBaseTypeId(TypeExpressions::Void)) {
      typeId = exprTypeId;
    }
    // if the expressionType is void, the lhs type can not be null, this case is caught above. if both types are defined but different, error
    else if (exprTypeId != typeTable.getBaseTypeId(TypeExpressions::Void)) {
      throw std::runtime_error(
          "type of variable not equal to rhs of assignment");
    }
  }


  auto symbol =
      VariableDefinitionSymbol{nesting, s.name, (unsigned)typeId, s.value};

  std::cout << "adding variable " << s.toString() << std::endl;
  variables.add(symbol);
}

void StatementVisitor::visitBlock(const Statements::BlockStatement& block) {
  nesting++;
  for (auto const& s : block.subNodes) {
    s.get()->accept(this);
  }
  nesting--;
  variables.removeAllLowerThan(nesting);
}
void ProgramVisitor::check() {
  for (const auto& function : program.functionDefinitions) {
    v.visitBlock(*std::move(function.statements));
    v.print();
  }
}

std::size_t ExpressionVisitor::getVariableType(const std::string& name) {
  if (auto var = variables.get(name)) {
    return (*var).type;
  }
  throw std::runtime_error("unknown variable");
}

}  // namespace Analysis
void Statements::BlockStatement::accept(StatementVisitor* visitor) {
  visitor->visitBlock(*this);
}
void Statements::VariableDefinitionStatement::accept(
    StatementVisitor* visitor) {
  visitor->visitVariableDefinition(*this);
}
std::size_t Expressions::Unary::getType(Analysis::ExpressionVisitor& c) const {
  // unary expressions, by default, do not change the type of an expression.
  // This can be overriden in special cases.
  return expression->getType(c);
}
std::size_t Expressions::Binary::getType(Analysis::ExpressionVisitor& c) const {
  auto lhsType = lhs->getType(c), rhsType = rhs->getType(c);
  if (rhsType != lhsType) {
    throw std::runtime_error("binary with different types not supported");
  }
  return lhsType;
}
std::size_t Expressions::BooleanLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return Analysis::TypeTable::getBaseTypeId(TypeExpressions::Boolean);
}
std::size_t Expressions::IntegerLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return Analysis::TypeTable::getBaseTypeId(TypeExpressions::Integer);
}
std::size_t Expressions::StringLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return Analysis::TypeTable::getBaseTypeId(TypeExpressions::String);
}
std::size_t Expressions::NullExpression::getType(
    Analysis::ExpressionVisitor& c) const {
  return Analysis::TypeTable::getBaseTypeId(TypeExpressions::Void);
}
std::size_t Expressions::VariableReference::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.getVariableType(variableName);
}

}  // namespace Ela
