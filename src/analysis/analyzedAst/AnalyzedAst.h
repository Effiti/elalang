// #include "../Type.h"
#include <memory>
#include <string_view>

#include "../Visitor.h"
namespace Ela::Analysis {
class Type;
class AnalyzedExpression {
 protected:
  AnalyzedExpression(std::shared_ptr<Type> t) : type{t} {};

 public:
  const std::shared_ptr<Type> type;
};

// Yes, this is currently very much Boilerplate, but
// I might need more functionality in the AST after
// Analysis for Codegen, which this will allow me to implement
// without cluttering the post-Parsing AST
// after all, we don't really want our language to be too
// close to LLVM IR, and this will allow us to change
// the AST dramatically during Analysis

class ABinaryExpression : public AnalyzedExpression {
 public:
  AnalyzedExpression lhs, rhs;
  BinaryOperatorType op;
  ABinaryExpression(AnalyzedExpression lhs, AnalyzedExpression rhs,
                    BinaryOperatorType op, std::shared_ptr<Type> t)
      : lhs(lhs), rhs(rhs), AnalyzedExpression(t), op(op) {};
};
class AUnaryExpression : public AnalyzedExpression {
 public:
  AnalyzedExpression expr;
  UnaryOperatorType op;
  AUnaryExpression(AnalyzedExpression expr, UnaryOperatorType op,
                   std::shared_ptr<Type> t)
      : AnalyzedExpression(t), expr(expr), op(op) {};
};
class AIntegerLiteral : public AnalyzedExpression {
 public:
  int value;
  AIntegerLiteral(int value, std::shared_ptr<Type> t)
      : AnalyzedExpression(t), value(value) {};
};
class ACharacterLiteral : public AnalyzedExpression {
 public:
  char value;
  ACharacterLiteral(char value, std::shared_ptr<Type> t)
      : value(value), AnalyzedExpression(t) {};
};
class AStringLiteral : public AnalyzedExpression {
 public:
  std::string_view value;
  AStringLiteral(std::string_view value, std::shared_ptr<Type> t)
      : value(value), AnalyzedExpression(t) {};
};
class ABooleanLiteral : public AnalyzedExpression {
 public:
  const bool value;
  ABooleanLiteral(bool value, std::shared_ptr<Type> t)
      : value(value), AnalyzedExpression(t) {};
};
class AFunctionCall : public AnalyzedExpression {
 public:
  const std::string_view functionName;
  const vector<std::shared_ptr<AnalyzedExpression>> params;
  AFunctionCall(std::string_view functionName,
                vector<std::shared_ptr<AnalyzedExpression>> params,
                std::shared_ptr<Type> t)
      : functionName(functionName), params(params), AnalyzedExpression(t) {};
};
class AArrayLiteral : public AnalyzedExpression {
 public:
  const vector<std::shared_ptr<AnalyzedExpression>> elements;
  AArrayLiteral(vector<std::shared_ptr<AnalyzedExpression>> elements,
                std::shared_ptr<Type> t)
      : elements(elements), AnalyzedExpression(t) {};
};

class AnalyzedStatement {};
class AElseStatement : public AnalyzedStatement {
 public:
  const std::shared_ptr<AnalyzedStatement> stmt;
};
class AIfStatement : public AnalyzedStatement {
 public:
  const std::shared_ptr<AnalyzedStatement> then;
  const std::shared_ptr<AnalyzedExpression> cond;
};
class AExpressionStatement : public AnalyzedStatement {
 public:
  const std::shared_ptr<AnalyzedExpression> expr;
};
class ABlockStatement : public AnalyzedStatement {
 public:
  const vector<std::shared_ptr<AnalyzedStatement>> statements;
  ABlockStatement(const vector<std::shared_ptr<AnalyzedStatement>> statements)
      : statements(statements) {};
};
class AReturnStatement : public AnalyzedStatement {
 public:
  const std::shared_ptr<AnalyzedExpression> expr;
  AReturnStatement(const std::shared_ptr<AnalyzedExpression> expr)
      : expr(expr) {};
};
class AVariableDefinitionStatement : public AnalyzedStatement {
 public:
  const std::string_view varName;
  const std::shared_ptr<Type> varType;
  const std::shared_ptr<AnalyzedExpression> varValue;
  AVariableDefinitionStatement(std::string_view varName,
                               std::shared_ptr<Type> varType,
                               std::shared_ptr<AnalyzedExpression> varValue)
      : varName(varName), varType(varType), varValue(varValue) {}
};
class AWhileStatement : public AnalyzedStatement {
 public:
  const std::shared_ptr<AnalyzedStatement> body;
  const std::shared_ptr<AnalyzedExpression> cond;
  AWhileStatement(std::shared_ptr<AnalyzedStatement> body,
                  std::shared_ptr<AnalyzedExpression> cond)
      : body{body}, cond{cond} {}
};
class AParameter {
 public:
  const std::string_view name;
  const std::shared_ptr<Type> type;
};
class AFunctionDefinition : public AnalyzedStatement {
 public:
  std::shared_ptr<Type> returnType;
  std::string_view functionName;
  vector<AParameter> parameters;
  std::shared_ptr<ABlockStatement> statements;
  bool isExtern;
  AFunctionDefinition(std::shared_ptr<Type> returnType,
                      std::string_view functionName,
                      std::vector<AParameter> parameters,
                      std::shared_ptr<ABlockStatement> statements,
                      bool isExtern)
      : returnType(std::move(returnType)),
        functionName(functionName),
        parameters(std::move(parameters)),
        statements(std::move(statements)),
        isExtern(isExtern) {}
};

class AProgram {
  // TODO: handle import statements, first just in a 'stupid' c-style include
  // way
 public:
  const vector<std::shared_ptr<AFunctionDefinition>> functions;
};

}  // namespace Ela::Analysis
