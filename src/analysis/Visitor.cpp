#include "Visitor.h"

#include <stdexcept>

#include "SymbolTable.h"

namespace Ela {
namespace Analysis {
void StatementVisitor::visitVariableDefinition(
    const Statements::VariableDefinitionStatement& s) {
  if (variables.hasSymbol(s.name))
    std::cerr << colors["yellow"] << "WARN: overriding local variable"
              << colors["end"] << std::endl;

  int typeId = typeTable.getType(s.type->toString());
  if (typeId == -1) {
    typeId = typeTable.add(TypeEntry(s.type->toString(), s.type));
  }
  int exprTypeId = s.value->getType(expressionVisitor);
  // if the types are not equal, there are two possibilities
  if (exprTypeId != typeId) {
    // if the type is supposed to be inferred, *only* the lhs type is Infer.
    if (typeId == typeTable.getBaseTypeId(TypeExpressions::Infer)) {
      typeId = exprTypeId;
    }
    // if the expressionType is void, the lhs type can not be null, this case is
    // caught above. if both types are defined but different, error
    else if (exprTypeId != typeTable.getBaseTypeId(TypeExpressions::Null)) {
      typeTable.print();
      throw std::runtime_error(
          "type of variable " + s.name +
          " not equal to rhs of assignment (comparing types " +
          s.type->toString() + ":" + std::to_string(typeId) + " (of lhs) and " +
          typeTable.getType((int)exprTypeId).type->toString() + ":" +
          std::to_string(exprTypeId) + " (of rhs) )");
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
  std::cout << "variables:" << std::endl;
  variables.print();
  std::cout << "types:" << std::endl;
  typeTable.print();

  nesting--;
  std::cout << "removing" << std::endl;

  variables.removeAllHigherThan(nesting);
}
void ProgramVisitor::check() {
  for (const auto& function : program.functionDefinitions) {
    std::vector<std::shared_ptr<TypeExpressions::TypeExpression>> paramTypes;

    for (const auto& param : function.parameters) {
      paramTypes.push_back(param.parameterType);
    }
    auto typeId = v.typeTable.getType(function.returnType->toString());
    if (typeId == -1) {
      v.typeTable.add(
          TypeEntry(function.returnType->toString(), function.returnType));
      typeId = v.typeTable.getType(function.returnType->toString());
    }
    std::vector<size_t> paramTypeIds;

    for (const auto& paramType : paramTypes) {
      std::size_t id = v.typeTable.getType(paramType->toString());
      if (id == -1) {
        v.typeTable.add(TypeEntry(paramType->toString(), paramType));
        id = v.typeTable.getType(paramType->toString());
      }
      paramTypeIds.push_back(id);
    }
    v.functions.add(FunctionDefinitionSymbol(
        (unsigned int)0, function.functionName, typeId, paramTypeIds));
  }
  for (const auto& function : program.functionDefinitions) {
    v.visitBlock(*std::move(function.statements));
  }
}

std::size_t ExpressionVisitor::getVariableType(const std::string& name) {
  if (auto var = variables.get(name)) {
    return (*var).type;
  }
  throw std::runtime_error("unknown variable " + name);
}

std::size_t ExpressionVisitor::getArrayType(const std::size_t baseType) {
  const auto& tr = types.getType(baseType);
  const auto& t = types.getType("Array[" + tr.typeStr + ", ]");
  if (t != -1) return t;
  types.add(TypeEntry(
      "Array[" + tr.typeStr + ", ]",
      std::make_shared<TypeExpressions::TypeExpression>(
          TypeExpressions::TypeTemplateExpression(
              TypeExpressions::SimpleType(TypeExpressions::Array),
              std::vector<std::variant<
                  std::shared_ptr<TypeExpressions::TypeExpression>, int>>{
                  tr.type}))));
  return types.getType("Array[" + tr.typeStr + ", ]");
}

}  // namespace Analysis
void Statements::BlockStatement::accept(StatementVisitor* visitor) {
  visitor->visitBlock(*this);
}
void Statements::IfStatement::accept(StatementVisitor* visitor) {
  if (condition->getType(visitor->expressionVisitor) !=
      Analysis::TypeTable::getBaseTypeId(TypeExpressions::Boolean))
    throw std::runtime_error(
        "condition of if statement must be of boolean type to avoid confusion");
  statement->accept(visitor);
}
void Statements::ExpressionStatement::accept(StatementVisitor* visitor) {
  const auto& type = expression->getType(visitor->expressionVisitor);
  if (visitor->typeTable.getBaseTypeId(TypeExpressions::Void) != type)
    throw std::runtime_error("unused Expression statement result");
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
  if (op == BinaryOperatorType::MemberAccess)
    throw std::runtime_error(
        "FIXME there's absolutely no way I'm implementing MemberAccess "
        "already");
  auto lhsType = lhs->getType(c), rhsType = rhs->getType(c);
  if (rhsType != lhsType) {
    throw std::runtime_error("binary with different types not supported");
  }
  switch (op) {
    case BinaryOperatorType::Equal:
      return Analysis::TypeTable::getBaseTypeId(TypeExpressions::Boolean);
    case BinaryOperatorType::Plus:
      return lhsType;
    default:
      return Analysis::TypeTable::getBaseTypeId(TypeExpressions::Void);
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
std::size_t Expressions::ArrayLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  if (elements.size() == 0)
    return Analysis::TypeTable::getBaseTypeId(TypeExpressions::BaseType::Array);
  const auto firstType = elements[0]->getType(c);

  for (const auto element : elements) {
    if (element->getType(c) != firstType)
      throw std::runtime_error("elements of array have different types");
  }
  return c.getArrayType(firstType);
}
std::size_t Expressions::FunctionCall::getType(
    Analysis::ExpressionVisitor& c) const {
  const auto& fnopt = c.functions.get(functionName);
  if (!fnopt) {
    throw std::runtime_error("unknown function");
  }
  Analysis::FunctionDefinitionSymbol fn = *fnopt;
  const std::size_t& returnType = fn.type;

  const std::vector<size_t>& argTypes = fn.argTypes;

  if(argTypes.size() != callParams.size()) 
    throw std::runtime_error("argument mismatch");

  std::size_t i = 0;
  for (const auto& argType : argTypes) {
    if (argType != callParams[i]->getType(c)) {
      throw std::runtime_error("argument type mismatch");
    }
    i++;
  }
  return returnType;
}
}  // namespace Ela
