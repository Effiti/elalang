#include "Visitor.h"

#include <memory>
#include <stdexcept>

#include "SymbolTable.h"

namespace Ela {
namespace Analysis {
void StatementVisitor::visitVariableDefinition(
    Statements::VariableDefinitionStatement& s) {
  if (variables.hasSymbol(s.name))
    std::cerr << colors["yellow"] << "WARN: overriding local variable"
              << colors["end"] << std::endl;

  int typeId = 0;
  int exprTypeId = s.value->getType(expressionVisitor);
  bool infer = s.type->isInfer();
  if (infer) {
    typeId = exprTypeId;
  } else {
    typeId = typeTable.getTypeId(s.type->toAnalysisType());
  }
  // if the expressionType is void, the lhs type can not be null, this case is
  // caught above. if both types are defined but different, error
  if (!infer && exprTypeId != typeTable.getBaseTypeId(TypeExpressions::Null)) {
    // typeTable.print();
    throw std::runtime_error(
        "type of variable " + s.name +
        " not equal to rhs of assignment (comparing types " +
        s.type->toString() + ":" + std::to_string(typeId) + " (of lhs) and " +
        typeTable.getType((int)exprTypeId).toString() + ":" +
        std::to_string(exprTypeId) + " (of rhs) )");
  }
  s.analysedType = std::optional(std::make_shared<Type>(expressionVisitor.types.getType(typeId)));
  

  auto symbol = std::make_shared<VariableDefinitionSymbol>(
      nesting, s.name, (unsigned)typeId, s.value);

  // std::cout << "adding variable " << s.toString() << std::endl;
  variables.add(*symbol);
  contextFn.vars.push_back(symbol);
}

void StatementVisitor::visitBlock(const Statements::BlockStatement& block,
                                  bool fnBlock) {
  nesting++;
  if (fnBlock) {
    for (const auto& param : contextFn.args)
      variables.add(
          VariableDefinitionSymbol(nesting, param.name, param.type,
                                   std::make_shared<Expressions::Expression>(
                                       Expressions::NullExpression())));
  }
  for (auto const& s : block.subNodes) {
    s.get()->accept(this);
  }
  // std::cout << "variables:" << std::endl;
  // variables.print();
  // std::cout << "types:" << std::endl;
  // typeTable.print();

  nesting--;
  // std::cout << "removing" << std::endl;

  variables.removeAllHigherThan(nesting);
}
void ProgramVisitor::check() {
  for (const auto& function : program.functionDefinitions) {
    std::vector<std::shared_ptr<TypeExpressions::TypeExpression>> paramTypes;

    for (const auto& param : function.parameters) {
      paramTypes.push_back(param.parameterType);
    }
    const auto type = function.returnType->toAnalysisType();
    auto typeId = v.typeTable.getTypeId(type);
    std::vector<size_t> paramTypeIds;

    for (const auto& paramType : paramTypes) {
      const auto type = paramType->toAnalysisType();
      std::size_t id = v.typeTable.getTypeId(type);
      paramTypeIds.push_back(id);
    }
    v.functions.add(FunctionDefinitionSymbol(
        (unsigned int)0, function.functionName, typeId, paramTypeIds));
  }
  for (Statements::FunctionDefinition& function : program.functionDefinitions) {
    std::vector<FunctionParameter> args{};
    const auto returnType = function.returnType->toAnalysisType();
    for (const auto& arg : function.parameters) {
      const auto argType = arg.parameterType->toAnalysisType();
      args.push_back(
          FunctionParameter(arg.parameterName, v.typeTable.getTypeId(argType)));
    }
    v.contextFn = IncompleteFunction(function.functionName,
                                     v.typeTable.getTypeId(returnType), args);
    v.visitBlock(*std::move(function.statements), true);
    function.addDecls(v.contextFn.vars);
  }
  // allocate memory for a typeTable with smartpointer in Program.
  program.typeTable = std::make_shared<TypeTable>(v.typeTable);
}

std::size_t ExpressionVisitor::getVariableType(const std::string& name) {
  if (auto var = variables.get(name)) {
    return (*var).type;
  }
  throw std::runtime_error("unknown variable " + name);
}

std::size_t ExpressionVisitor::getArrayType(const std::size_t baseTypeId) {
  const auto& baseType = types.getType(baseTypeId);
  return types.getTypeId(Type(ArrayType(std::make_shared<Type>(baseType))));
}

std::size_t ExpressionVisitor::getPointerType(const std::size_t baseTypeId) {
  const auto& baseType = types.getType(baseTypeId);
  return types.getTypeId(Type(PointerType(std::make_shared<Type>(baseType))));
}
}  // namespace Analysis
void Statements::BlockStatement::accept(StatementVisitor* visitor) {
  visitor->visitBlock(*this, false);
}
void Statements::IfStatement::accept(StatementVisitor* visitor) {
  if (condition->getType(visitor->expressionVisitor) !=
      visitor->expressionVisitor.types.getBaseTypeId(TypeExpressions::Boolean))
    throw std::runtime_error(
        "condition of if statement must be of boolean type to avoid confusion");
  statement->accept(visitor);
}
void Statements::WhileStatement::accept(Analysis::StatementVisitor* visitor) {
  if (cond->getType(visitor->expressionVisitor) !=
      visitor->expressionVisitor.types.getBaseTypeId(TypeExpressions::Boolean))
    throw std::runtime_error(
        "condition of while statement must be of boolean type to avoid "
        "confusion");
  body->accept(visitor);
}
void Statements::ForStatement::accept(Analysis::StatementVisitor* visitor) {
  init->accept(visitor);
  if (check->getType(visitor->expressionVisitor) !=
      visitor->expressionVisitor.types.getBaseTypeId(TypeExpressions::Boolean))
    throw std::runtime_error(
        "check-condition of for statement must be of boolean type to avoid "
        "confusion");
  incr->accept(visitor);
  body->accept(visitor);
}

void Statements::ExpressionStatement::accept(StatementVisitor* visitor) {
  const auto& type = expression->getType(visitor->expressionVisitor);
  // if (visitor->typeTable.getBaseTypeId(TypeExpressions::Void) != type)
  //   throw std::runtime_error("unused Expression statement result");
}
void Statements::ReturnStatement::accept(StatementVisitor* visitor) {
  if (expression->getType(visitor->expressionVisitor) !=
      visitor->contextFn.returnType)
    throw std::runtime_error(
        "return statements must return the correct type in fn " +
        visitor->contextFn.name + " expected " +
        visitor->typeTable.getType(visitor->contextFn.returnType).toString() +
        " but got " +
        visitor->typeTable
            .getType(expression->getType(visitor->expressionVisitor))
            .toString());
}
void Statements::VariableDefinitionStatement::accept(
    StatementVisitor* visitor) {
  visitor->visitVariableDefinition(*this);
}
std::size_t Expressions::Unary::getType(Analysis::ExpressionVisitor& c) const {
  // unary expressions, by default, do not change the type of an expression.
  // This can be overriden in special cases.
  if (op == UnaryOperatorType::Address) {
    return c.getPointerType(expression->getType(c));
  }
  return expression->getType(c);
}
std::size_t Expressions::Parenthed::getType(
    Analysis::ExpressionVisitor& c) const {
  return subExpr->getType(c);
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
    case BinaryOperatorType::GreaterEqual:
    case BinaryOperatorType::Less:
    case BinaryOperatorType::LessEqual:
    case BinaryOperatorType::UnEqual:
      return c.types.getBaseTypeId(TypeExpressions::Boolean);
    case BinaryOperatorType::Plus:
    case BinaryOperatorType::Minus:
    case BinaryOperatorType::Multiplication:
    case BinaryOperatorType::Division:
    case BinaryOperatorType::LeftShift:
    case BinaryOperatorType::RightShift:
      return lhsType;
    default:
      return c.types.getBaseTypeId(TypeExpressions::Void);
  }
  return lhsType;
}
std::size_t Expressions::BooleanLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.types.getBaseTypeId(BaseType::Boolean);
}
std::size_t Expressions::IntegerLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.types.getBaseTypeId(BaseType::Integer);
}
std::size_t Expressions::CharacterLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.types.getBaseTypeId(BaseType::Char);
}
std::size_t Expressions::StringLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.types.TypeTable::getBaseTypeId(BaseType::CString);
}
std::size_t Expressions::NullExpression::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.types.getBaseTypeId(BaseType::Void);
}
std::size_t Expressions::VariableReference::getType(
    Analysis::ExpressionVisitor& c) const {
  return c.getVariableType(variableName);
}
std::size_t Expressions::VariableAssign::getType(
    Analysis::ExpressionVisitor& c) const {
  std::size_t valType = value->getType(c);
  if (c.getVariableType(name) != valType) {
    throw std::runtime_error("assignment lhs type not equal to rhs type");
  }
  return value->getType(c);
}
std::size_t Expressions::ArrayLiteral::getType(
    Analysis::ExpressionVisitor& c) const {
  if (elements.size() == 0)
    return c.types.getBaseTypeId(TypeExpressions::BaseType::Array);
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

  if (argTypes.size() != callParams.size())
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
