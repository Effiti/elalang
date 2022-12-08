#pragma once

#include <utility>

#include "Ela.hpp"
#include "Node.hpp"

enum class OperatorType {
    None,
    Multiplication,
    Plus,
    Minus,
    Division,
    RightShift,
    LeftShift

};
namespace TypeExpressions {
    class TypeExpression : public NonLeafNode {


    };

    class PrimaryTypeExpression : public TypeExpression {

    };

    class TypeVariable : public PrimaryTypeExpression {

    };
}


namespace Expressions {


    class Expression : public Node {
    public:
        virtual const bool isConstEvaluable();
    };

    class Binary : public Expression {
    private:
        Expression mLhs;
        Expression mRhs;
        OperatorType mOp;

    public:
        Binary(Expression rhs, OperatorType op, Expression lhs)
                : mLhs{lhs}, mRhs{std::move(rhs)}, mOp{op} {};

        const bool isConstEvaluable() override;
    };

    class PrimaryExpression : public Expression {
    };

    class Parenthed : public PrimaryExpression {
    public:
        explicit Parenthed(Expression pSubExpr) : SubExpr{pSubExpr}{};

        Expression SubExpr;
    };

    class IntegerLiteral : public PrimaryExpression {
    public:
        IntegerLiteral(int v) : value{v}{};

        int value;

        const bool isConstEvaluable() { return true; }
    };
} // namespace Expressions
