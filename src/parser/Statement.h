#pragma once

#include <memory>
#include <utility>

#include "./Expression.h"
#include "./TypeExpression.h"

namespace Ela::Analysis {
  class StatementVisitor;
};

namespace Ela::Statements {
using namespace Ela::Analysis;
class ImportStatement : public Node {
 public:
  explicit ImportStatement(std::string m) : mod{std::move(m)} {}

  std::string mod;
};

class Statement : public Node {
 public:
  virtual void accept(Analysis::StatementVisitor* visitor){};
  virtual const std::string toString() const;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::shared_ptr<Expressions::Expression> cond,
              std::shared_ptr<Statement> stmt)
      : condition{std::move(cond)}, statement{std::move(stmt)} {};
  std::shared_ptr<Expressions::Expression> condition;
  std::shared_ptr<Statement> statement;

  const string toString() const override;
};

class ElseStatement : public Statement {
 public:
  ElseStatement(std::shared_ptr<Statement> stmt)
      : statement{std::move(stmt)} {};
  std::shared_ptr<Statement> statement;

  const std::string toString() const override;
};

class ExpressionStatement : public Statement {
 public:
  ExpressionStatement(std::shared_ptr<Expressions::Expression> expr)
      : expression{std::move(expr)} {};
  std::shared_ptr<Expressions::Expression> expression;

  const std::string toString() const override;
};

class BlockStatement : public Statement {
 public:
  explicit BlockStatement(vector<std::shared_ptr<Statement>> nodes)
      : subNodes(std::move(nodes)) {}

  std::vector<std::shared_ptr<Statement>> subNodes;

  void accept(StatementVisitor * visitor) override;
  const string toString() const override;
};

class ReturnStatement : public Statement {
 public:
  ReturnStatement(std::shared_ptr<Expressions::Expression> expr)
      : expression{std::move(expr)} {};
  std::shared_ptr<Expressions::Expression> expression;

  const string toString() const override;
};

class VariableDefinitionStatement : public Statement {
 public:
  VariableDefinitionStatement(
      std::string varName,
      std::shared_ptr<TypeExpressions::TypeExpression> varType,
      std::shared_ptr<Expressions::Expression> varValue)
      : name(std::move(varName)),
        type(std::move(varType)),
        value(std::move(varValue)){};

  const std::string name;
  std::shared_ptr<TypeExpressions::TypeExpression> type;
  std::shared_ptr<Expressions::Expression> value;

  void accept(StatementVisitor * visitor) override;
  const std::string toString() const override;
};

class Parameter : public Node {
  ;

 public:
  std::string parameterName;
  std::shared_ptr<TypeExpressions::TypeExpression> parameterType;

  Parameter(std::shared_ptr<TypeExpressions::TypeExpression> type,
            std::string name)
      : parameterType{std::move(type)}, parameterName{std::move(name)} {}
};

// this class does not end in ...Statement as it is not technically a statement:
// all statements can occur inside a block but a function definition may not.
class FunctionDefinition : public Statement {
 public:
  FunctionDefinition(std::shared_ptr<TypeExpressions::TypeExpression> ret,
                     std::string name, vector<Parameter> params,
                     BlockStatement code)
      : functionName{std::move(name)},
        parameters{std::move(params)},
        statements(std::make_shared<BlockStatement>(code)),
        returnType{std::move(ret)} {};
  std::shared_ptr<TypeExpressions::TypeExpression> returnType;
  const std::string functionName;
  vector<Parameter> parameters;
  std::shared_ptr<BlockStatement> statements;
};

class Program : public Node {
 public:
  Program() = default;
  ;
  Program(vector<ImportStatement> iS, vector<FunctionDefinition> fD)
      : importStatements{std::move(iS)}, functionDefinitions{std::move(fD)} {}
  vector<ImportStatement> importStatements;
  vector<FunctionDefinition> functionDefinitions;
};
}  // namespace Ela::Statements
