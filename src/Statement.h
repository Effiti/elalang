#pragma once

#include <utility>
#include <memory>

#include "Expression.h"
#include "TypeExpression.h"

namespace Statements {
    class ImportStatement : public Node {
    public:
        ImportStatement(std::string m) :
                mod{std::move(m)} {}

        std::string mod;
    };

    class Statement : public Node {
    };

    class Parameter : public Node {
        Parameter(TypeExpressions::TypeExpression type, std::string_view name) : parameterType{type},
                                                                                 parameterName{name} {};
    public:
        std::string_view parameterName;
        TypeExpressions::TypeExpression parameterType;
    };

    class FunctionDefinition : public Statement {
    public:
        FunctionDefinition(TypeExpressions::TypeExpression *ret, std::string_view name,
                           SameTypeNodeList<Parameter> params) : functionName{name},
                                                                 parameters{std::move(params)} { returnType = ret;};
        TypeExpressions::TypeExpression *returnType;
        std::string_view functionName;
        SameTypeNodeList<Parameter> parameters;
    };

    class Programm : public Node {
    public:
        SameTypeNodeList<ImportStatement> importStatements;
        SameTypeNodeList<FunctionDefinition> functionDefinitions;
    };
}