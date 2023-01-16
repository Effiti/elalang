#pragma once

#include <utility>
#include <memory>

#include "Expression.h"
#include "TypeExpression.h"

namespace Ela::Statements {
    class ImportStatement : public Node {
    public:
        explicit ImportStatement(std::string m) :
                mod{std::move(m)} {}

        std::string mod;
    };

    class Statement : public Node {
    public:
        virtual std::string toString();
    };

    class IfStatement : public Statement {
    public:
        IfStatement(std::unique_ptr<Expressions::Expression> cond, std::unique_ptr<Statement> stmt) : condition{
                std::move(cond)}, statement{std::move(stmt)} {};
        std::unique_ptr<Expressions::Expression> condition;
        std::unique_ptr<Statement> statement;

        string toString() override;
    };

    class ElseStatement : public Statement {
    public:
        ElseStatement(std::unique_ptr<Statement> stmt) : statement{std::move(stmt)} {};
        std::unique_ptr<Statement> statement;

        std::string toString() override;
    };

    class ExpressionStatement : public Statement {
    public:
        ExpressionStatement(std::unique_ptr<Expressions::Expression> expr) : expression{std::move(expr)} {};
        std::unique_ptr<Expressions::Expression> expression;

        std::string toString() override;
    };

    class BlockStatement : public Statement {
    public:
        explicit BlockStatement(vector<std::unique_ptr<Statement>> nodes) : subNodes(std::move(nodes)) {}

        std::vector<std::unique_ptr<Statement>> subNodes;

        string toString() override;
    };

    class ReturnStatement : public Statement {
    public:
        ReturnStatement(std::unique_ptr<Expressions::Expression> expr) : expression{std::move(expr)} {};
        std::unique_ptr<Expressions::Expression> expression;

        string toString() override;
    };

    class VariableDefinitionStatement : public Statement {
    public:
        VariableDefinitionStatement(std::string varName, std::unique_ptr<TypeExpressions::TypeExpression> varType,
                                    std::unique_ptr<Expressions::Expression> varValue) : name(std::move(varName)),
                                                                         type(std::move(varType)),
                                                                         value(std::move(varValue)) {};

        const std::string name;
        std::unique_ptr<TypeExpressions::TypeExpression> type;
        std::unique_ptr<Expressions::Expression> value;

        std::string toString() override;
    };

    class Parameter : public Node { ;
    public:
        std::string parameterName;
        std::unique_ptr<TypeExpressions::TypeExpression> parameterType;

        Parameter(std::unique_ptr<TypeExpressions::TypeExpression> type, std::string name) : parameterType{std::move(type)},
                                                                             parameterName{std::move(name)} {}
    };

    // this class does not end in ...Statement as it is not technically a statement:
    // all statements can occur inside a block but a function definition may not.
    class FunctionDefinition : public Statement {
    public:
        FunctionDefinition(std::unique_ptr<TypeExpressions::TypeExpression> ret, std::string name,
                           vector<Parameter> params, std::unique_ptr<Statement> code) :
                           functionName{std::move(name)},
                           parameters{std::move(params)},
                           statements(std::move(code)),
                           returnType{std::move(ret)} {};
        std::unique_ptr<TypeExpressions::TypeExpression> returnType;
        const std::string functionName;
        vector<Parameter> parameters;
        std::unique_ptr<Statement> statements;
    };

    class Program : public Node {
    public:
        Program() = default;;
        Program(vector<ImportStatement> iS, vector<FunctionDefinition> fD) : importStatements{std::move(iS)}, functionDefinitions{std::move(fD)} {}
        vector<ImportStatement> importStatements;
        vector<FunctionDefinition> functionDefinitions;
    };
}