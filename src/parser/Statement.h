#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <utility>

#include "./Expression.h"
#include "./TypeExpression.h"

namespace Ela::Analysis {
class StatementVisitor;
class VariableDefinitionSymbol;
class TypeTable;
class AnalyzedStatement;
class AProgram;
class AFunctionDefinition;
class ABlockStatement;
};  // namespace Ela::Analysis
namespace Ela::Emitter {
class Emitter;
}

namespace Ela::Statements {
using namespace Ela::Analysis;
class ImportStatement : public Node {
 public:
  explicit ImportStatement(std::string m) : mod{std::move(m)} {}

  std::string mod;
};

class Statement : public Node {
 public:
  virtual void accept(Analysis::StatementVisitor* visitor) {};
  virtual const std::string toString() const;
  virtual llvm::Value* codegen(Emitter::Emitter& e) { return nullptr; }
  virtual const bool is_returning() const { return false; }
  virtual Analysis::AnalyzedStatement toAnalyzed(StatementVisitor&) const;
};

class ElseStatement : public Statement {
 public:
  ElseStatement(std::shared_ptr<Statement> stmt)
      : statement{std::move(stmt)} {};
  std::shared_ptr<Statement> statement;

  const std::string toString() const override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::shared_ptr<Expressions::Expression> cond,
              std::shared_ptr<Statement> stmt)
      : condition{std::move(cond)}, statement{std::move(stmt)} {};
  std::shared_ptr<Expressions::Expression> condition;
  std::shared_ptr<Statement> statement;
  std::optional<std::shared_ptr<ElseStatement>> elseStatement;

  void accept(Analysis::StatementVisitor* visitor) override;
  const string toString() const override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
  // const bool is_return() const override;
};

class ExpressionStatement : public Statement {
 public:
  ExpressionStatement(std::shared_ptr<Expressions::Expression> expr)
      : expression{std::move(expr)} {};
  std::shared_ptr<Expressions::Expression> expression;

  void accept(Analysis::StatementVisitor* visitor) override;
  const std::string toString() const override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
};

class BlockStatement : public Statement {
 public:
  explicit BlockStatement(vector<std::shared_ptr<Statement>> nodes)
      : subNodes(std::move(nodes)) {}

  std::vector<std::shared_ptr<Statement>> subNodes;

  void accept(StatementVisitor* visitor) override;
  const string toString() const override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
  ABlockStatement toAnalyzedBlock(StatementVisitor&) const;
  const bool is_returning() const override;
};
BlockStatement emptyBlock();
class ReturnStatement : public Statement {
 public:
  ReturnStatement(std::shared_ptr<Expressions::Expression> expr)
      : expression{std::move(expr)} {};
  std::shared_ptr<Expressions::Expression> expression;

  const bool is_returning() const override { return true; }
  const string toString() const override;
  void accept(Analysis::StatementVisitor* visitor) override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
};

class VariableDefinitionStatement : public Statement {
 public:
  VariableDefinitionStatement(
      std::string varName,
      std::shared_ptr<TypeExpressions::TypeExpression> varType,
      std::shared_ptr<Expressions::Expression> varValue)
      : name(std::move(varName)),
        type(std::move(varType)),
        value(std::move(varValue)) {};

  const std::string name;
  std::shared_ptr<TypeExpressions::TypeExpression> type;
  std::shared_ptr<Expressions::Expression> value;
  std::optional<std::shared_ptr<Analysis::Type>> analysedType;

  void accept(StatementVisitor* visitor) override;
  const std::string toString() const override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
};

class ForStatement : public Statement {
 public:
  ForStatement(const std::shared_ptr<Statement> init,
               const std::shared_ptr<Expressions::Expression> check,
               const std::shared_ptr<Statement> incr,
               const std::shared_ptr<Statement> body)
      : init{init}, check{check}, incr{incr}, body{body} {};
  const std::shared_ptr<Statement> init;
  const std::shared_ptr<Expressions::Expression> check;
  const std::shared_ptr<Statement> incr;
  const std::shared_ptr<Statement> body;

  void accept(StatementVisitor* visitor) override;
  const std::string toString() const override;
  llvm::Value* codegen(Emitter::Emitter& e) override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
  const bool is_returning() const override;
};

class WhileStatement : public Statement {
 public:
  WhileStatement(const std::shared_ptr<Expressions::Expression> cond,
                 const std::shared_ptr<Statement> body)
      : body{body}, cond{cond} {};
  const std::shared_ptr<Statement> body;
  const std::shared_ptr<Expressions::Expression> cond;

  void accept(StatementVisitor* visitor) override;
  const std::string toString() const override;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
  const bool is_returning() const override;
};

class Parameter : public Node {
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
                     BlockStatement code, bool isExtern)
      : functionName{std::move(name)},
        parameters{std::move(params)},
        statements(std::make_shared<BlockStatement>(code)),
        returnType{std::move(ret)},
        isExtern{isExtern} {};
  std::shared_ptr<TypeExpressions::TypeExpression> returnType;
  const std::string functionName;
  vector<Parameter> parameters;
  std::shared_ptr<BlockStatement> statements;
  bool isExtern;
  vector<std::shared_ptr<Analysis::VariableDefinitionSymbol>> decls;
  void addDecls(
      std::vector<std::shared_ptr<Analysis::VariableDefinitionSymbol>> vars) {
    for (auto& v : vars) decls.push_back(v);
  }
  AFunctionDefinition toAnalyzedFunction(StatementVisitor&) const;
  AnalyzedStatement toAnalyzed(StatementVisitor&) const override;
  // AFunctionDefinition toAnalyzedFunction(StatementVisitor&) const;
};

class Program : public Node {
 public:
  Program() = default;
  Program(vector<ImportStatement> iS, vector<FunctionDefinition> fD)
      : importStatements{std::move(iS)}, functionDefinitions{std::move(fD)} {}
  vector<ImportStatement> importStatements;
  vector<FunctionDefinition> functionDefinitions;
  std::shared_ptr<Analysis::TypeTable> typeTable;
  Analysis::AProgram toAnalyzed(Analysis::StatementVisitor&);
};
}  // namespace Ela::Statements
