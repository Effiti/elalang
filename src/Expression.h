#pragma once

#include <utility>

#include "Ela.hpp"
#include "Node.h"

enum class OperatorType {
    None,
    Multiplication,
    Plus,
    Minus,
    Division,
    RightShift,
    LeftShift

};

namespace Expressions {


    class Expression : public Node {
    public:
        virtual const bool isConstEvaluable();
        virtual std::string toString();
    };

    class Binary : public Expression {
    private:


    public:
        Binary(Expression *left, OperatorType operatorType, Expression *right)
                : lhs{left}, rhs{right}, op{operatorType} {};

        Expression *lhs;
        Expression *rhs;
        OperatorType op;

        const bool isConstEvaluable() override;
    };

    class Primary: public Expression {
    };

    class Parenthed : public Primary{
    public:
        explicit Parenthed(Expression *pSubExpr) : subExpr{pSubExpr}{};
        std::string toString() override;

        Expression *subExpr;
    };

    class IntegerLiteral : public Primary{
    public:
        IntegerLiteral(int v) : value{v}{};

        int value;
        std::string toString() override;

        const bool isConstEvaluable() override { return true; }
    };

    class NullExpression : public Primary {
        
    };
} // namespace Expressions
