#pragma once

#include <utility>
#include <memory>

#include "Expression.h"
#include "TypeExpression.h"

namespace Statements {
    class ImportStatement : public Node {
    public:
        explicit ImportStatement(std::string m) :
                mod{std::move(m)} {}

        std::string mod;
    };

    class Statement : public Node {
    };

    class BlockStatement : public Statement {
    public:
        BlockStatement(vector<Statement *> nodes) : subNodes(std::move(nodes)) {}

        std::vector<Statement *> subNodes;
    };

    class VariableDefinitionStatement : public Statement {
    public:
        VariableDefinitionStatement(std::string varName, TypeExpressions::TypeExpression *varType,
                                    Expressions::Expression *varValue) : name(std::move(varName)),
                                                                        type(varType),
                                                                        value(varValue) {};

        const std::string name;
        const TypeExpressions::TypeExpression *type;
        const Expressions::Expression *value;
    };

    class Parameter : public Node { ;
    public:
        std::string parameterName;
        TypeExpressions::TypeExpression *parameterType;

        Parameter(TypeExpressions::TypeExpression *type, std::string name) : parameterType{type},
                                                                             parameterName{std::move(name)} {}
    };
    // this class does not end in ...Statement as it is not technically a statement:
    // all statements can occur inside a block but a function definition may not.
    class FunctionDefinition : public Statement {
    public:
        FunctionDefinition(TypeExpressions::TypeExpression *ret, std::string name,
                           vector<Parameter> params, BlockStatement code) : functionName{std::move(name)},
                                                                            parameters{std::move(params)}, statements(
                        std::move(code)) { returnType = ret; };
        TypeExpressions::TypeExpression *returnType;
        const std::string functionName;
        vector<Parameter> parameters;
        BlockStatement statements;
    };

    class Programm : public Node {
    public:
        vector<ImportStatement> importStatements;
        vector<FunctionDefinition> functionDefinitions;
    };
}