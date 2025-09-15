#include "AnalyzedAst.h"

#include <memory>
#include <stdexcept>
#include <string_view>
namespace Ela::Expressions {
Analysis::AnalyzedExpression Unary::toAnalyzed(Analysis::ExpressionVisitor &e) {
  const auto expr = expression->toAnalyzed(e);
  return Analysis::AUnaryExpression(expr, op, e.getUnaryType(*expr.type, op));
}
Analysis::AnalyzedExpression Binary::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  auto alhs = lhs->toAnalyzed(e), arhs = rhs->toAnalyzed(e);
  return Analysis::ABinaryExpression(
      alhs, arhs, op, e.getBinaryType(*alhs.type, *arhs.type, op));
}
Analysis::AnalyzedExpression Parenthed::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  // Parentheses are just a syntactical concept to convey structural semantics,
  // which are encoded in this AST anyway
  return subExpr->toAnalyzed(e);
}
Analysis::AnalyzedExpression IntegerLiteral::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  return Analysis::AIntegerLiteral(
      value, std::make_shared<Analysis::Type>(
                 Analysis::Type(FundamentalType(BaseType::Integer))));
}
Analysis::AnalyzedExpression CharacterLiteral::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  return Analysis::ACharacterLiteral(
      value, std::make_shared<Analysis::Type>(
                 Analysis::Type(FundamentalType(BaseType::Char))));
}
Analysis::AnalyzedExpression StringLiteral::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  // TODO
  return Analysis::AStringLiteral(
      value, std::make_shared<Analysis::Type>(
                 Analysis::Type(FundamentalType(BaseType::CString))));
}
Analysis::AnalyzedExpression BooleanLiteral::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  return Analysis::ABooleanLiteral(
      value, std::make_shared<Analysis::Type>(
                 Analysis::Type(FundamentalType(BaseType::Boolean))));
}

Analysis::AnalyzedExpression FunctionCall::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  vector<std::shared_ptr<Analysis::AnalyzedExpression>> aparams;
  for (const auto &ptr : callParams) {
    aparams.push_back(
        std::make_shared<Analysis::AnalyzedExpression>(ptr->toAnalyzed(e)));
  }
  return Analysis::AFunctionCall(
      functionName, aparams,
      std::make_shared<Analysis::Type>(
          Analysis::Type(FundamentalType(BaseType::Boolean))));
}
Analysis::AnalyzedExpression Expression::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  throw std::runtime_error(
      "seems like this Expression is not implemented for analysis");
}
Analysis::AnalyzedExpression ArrayLiteral::toAnalyzed(
    Analysis::ExpressionVisitor &e) {
  vector<std::shared_ptr<Analysis::AnalyzedExpression>> aExprs;
  for (const auto &expr : elements) {
    aExprs.push_back(
        std::make_shared<Analysis::AnalyzedExpression>(expr->toAnalyzed(e)));
  }
  bool typesEqual = std::all_of(
      aExprs.begin(), aExprs.end(),
      [&aExprs](const auto &e) { return e->type == aExprs.front()->type; });
  if (!typesEqual) {
    throw std::runtime_error("unequal array element types");
  }
  return Analysis::AArrayLiteral{aExprs, aExprs.front()->type};
}
}  // namespace Ela::Expressions
namespace Ela::Statements {
Analysis::AnalyzedStatement Statement::toAnalyzed(StatementVisitor &c) const {
  throw std::runtime_error("unimplemented analyzed Statement");
}
Analysis::AProgram Program::toAnalyzed(Analysis::StatementVisitor &c) {
  std::vector<std::shared_ptr<AFunctionDefinition>> aFunctions;
  for (const auto &def : functionDefinitions) {
    aFunctions.push_back(
        std::make_shared<AFunctionDefinition>(def.toAnalyzedFunction(c)));
  }
  return AProgram{aFunctions};
}
Analysis::AFunctionDefinition FunctionDefinition::toAnalyzedFunction(
    StatementVisitor &c) const {
  vector<AParameter> aParams;
  for (const auto &param : parameters) {
    aParams.push_back(AParameter{
        param.parameterName,
        std::make_shared<Type>(param.parameterType->toAnalysisType())});
  }
  return AFunctionDefinition(
      std::make_shared<Type>(returnType->toAnalysisType()),
      std::string_view(functionName), aParams,
      std::make_shared<ABlockStatement>(statements->toAnalyzedBlock(c)),
      isExtern);
}
Analysis::AnalyzedStatement FunctionDefinition::toAnalyzed(
    StatementVisitor &c) const {
  return toAnalyzedFunction(c);
}
Analysis::AnalyzedStatement WhileStatement::toAnalyzed(
    StatementVisitor &c) const {
  return AWhileStatement(
      std::make_shared<AnalyzedStatement>(body->toAnalyzed(c)),
      std::make_shared<AnalyzedExpression>(
          cond->toAnalyzed(c.expressionVisitor)));
}

Analysis::AnalyzedStatement ForStatement::toAnalyzed(
    StatementVisitor &c) const {
  return AForStatement(
      std::make_shared<AnalyzedStatement>(init->toAnalyzed(c)),
      std::make_shared<AnalyzedExpression>(
          check->toAnalyzed(c.expressionVisitor)),
      std::make_shared<AnalyzedStatement>(incr->toAnalyzed(c)),
      std::make_shared<AnalyzedStatement>(body->toAnalyzed(c)));
}

Analysis::AnalyzedStatement VariableDefinitionStatement::toAnalyzed(
    StatementVisitor &c) const {
  return AVariableDefinitionStatement(
      name, std::make_shared<Type>(type->toAnalysisType()),
      std::make_shared<AnalyzedExpression>(
          value->toAnalyzed(c.expressionVisitor)));
}
Analysis::AnalyzedStatement ReturnStatement::toAnalyzed(
    StatementVisitor &c) const {
  return AReturnStatement(std::make_shared<AnalyzedExpression>(
      expression->toAnalyzed(c.expressionVisitor)));
}
Analysis::AnalyzedStatement BlockStatement::toAnalyzed(
    StatementVisitor &c) const {
  return toAnalyzedBlock(c);
}
Analysis::AnalyzedStatement IfStatement::toAnalyzed(StatementVisitor &c) const {
  AnalyzedStatement aElse =
      elseStatement.has_value()
          // we just take the underlying statement, because this way we don't
          // have to implement anything about else-stmts at all.
          ? elseStatement.value()->statement->toAnalyzed(c)
          : ANoOpStatement();
  return AIfStatement(
      std::make_shared<AnalyzedExpression>(
          condition->toAnalyzed(c.expressionVisitor)),
      std::make_shared<AnalyzedStatement>(statement->toAnalyzed(c)),
      std::make_shared<AnalyzedStatement>(aElse));
}
Analysis::AnalyzedStatement ElseStatement::toAnalyzed(
    StatementVisitor &c) const {
  return ANoOpStatement();
}
Analysis::AnalyzedStatement ExpressionStatement::toAnalyzed(
    StatementVisitor &c) const {
  return AExpressionStatement(std::make_shared<AnalyzedExpression>(
      expression->toAnalyzed(c.expressionVisitor)));
}
Analysis::ABlockStatement BlockStatement::toAnalyzedBlock(
    StatementVisitor &c) const {
  std::vector<std::shared_ptr<AnalyzedStatement>> statements;
  for (const auto &statement : subNodes) {
    statements.push_back(
        std::make_shared<AnalyzedStatement>(statement->toAnalyzed(c)));
  }
  return ABlockStatement(statements);
}
}  // namespace Ela::Statements
namespace Ela::Analysis {

std::shared_ptr<Type> ExpressionVisitor::getUnaryType(
    const Type &base, const UnaryOperatorType &op) const {
  // unary expressions, by default, do not change the type of an expression.
  // This can be overriden in special cases.
  if (op == UnaryOperatorType::Address) {
    return std::make_shared<Type>(
        Type(PointerType(std::make_shared<Type>(base))));
  }
  return std::make_shared<Type>(base);
}
std::shared_ptr<Type> ExpressionVisitor::getBinaryType(
    const Type &lhs, const Type &rhs, const BinaryOperatorType &op) const {
  if (op == BinaryOperatorType::MemberAccess)
    throw std::runtime_error(
        "FIXME there's absolutely no way I'm implementing MemberAccess "
        "already");
  if (rhs != lhs) {
    throw std::runtime_error("binary with different types not supported");
  }
  switch (op) {
    case BinaryOperatorType::Equal:
    case BinaryOperatorType::GreaterEqual:
    case BinaryOperatorType::Less:
    case BinaryOperatorType::LessEqual:
    case BinaryOperatorType::UnEqual:
      return std::make_shared<Type>(Type(FundamentalType(BaseType::Boolean)));
    case BinaryOperatorType::Plus:
    case BinaryOperatorType::Minus:
    case BinaryOperatorType::Multiplication:
    case BinaryOperatorType::Division:
    case BinaryOperatorType::LeftShift:
    case BinaryOperatorType::RightShift:
      return std::make_shared<Type>(lhs);
    default:
      return std::make_shared<Type>(Type(FundamentalType(BaseType::Void)));
  }
}
}  // namespace Ela::Analysis
