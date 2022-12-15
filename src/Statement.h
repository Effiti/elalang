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
        SameTypeNodeList<Statement*> subNodes;
    };

    class Parameter : public Node { ;
    public:
        std::string parameterName;
        TypeExpressions::TypeExpression *parameterType;

        Parameter(TypeExpressions::TypeExpression *type, std::string name) : parameterType{type},
                                                                                 parameterName{std::move(name)} {}
    };

    class FunctionDefinition : public Statement {
    public:
        FunctionDefinition(TypeExpressions::TypeExpression *ret, std::string name,
                           SameTypeNodeList<Parameter> params, BlockStatement code) : functionName{std::move(name)},
                                                                 parameters{std::move(params)}, statements(std::move(code)) { returnType = ret;};
        TypeExpressions::TypeExpression *returnType;
        const std::string functionName;
        SameTypeNodeList<Parameter> parameters;
        BlockStatement statements;
    };

    class Programm : public Node {
    public:
        SameTypeNodeList<ImportStatement> importStatements;
        SameTypeNodeList<FunctionDefinition> functionDefinitions;
    };
}