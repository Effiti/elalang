#include "Token.h"
namespace Ela::Lexing {
    std::string humanReadableTokenType(TokenType type) {
        using
        enum TokenType;
        switch (type) {
            case AssignmentOperator:
                return "AssignmentOperator";
            case LCurly:
                return "LCurly";
            case RCurly:
                return "RCurly";
            case RBracket:
                return "RBracket";
            case LBracket:
                return "LBracket";
            case DivisionOperator:
                return "DivisionOperator";
            case EndOfFile:
                return "EndOfFile";
            case EqualsOperator:
                return "EqualsOperator";
            case ExplicitConstQualifier:
                return "ExplicitConstQualifier";
            case GreaterThanOperator:
                return "GreaterThanOperator";
            case FunctionKeyword:
                return "FunctionKeyword";
            case IfKeyword:
                return "IfKeyword";
            case ElseKeyword:
                return "ElseKeyword";
            case Identifier:
                return "Identifier";
            case ImportKeyword:
                return "ImportKeyword";
            case ReturnKeyword:
                return "ReturnKeyword";
            case LessThanOperator:
                return "LessThanOperator";
            case Hyphen:
                return "Hyphen";
            case Asterisk:
                return "Asterisk";
            case NumberLiteral:
                return "NumberLiteral";
            case RParen:
                return "RParen";
            case LParen:
                return "LParen";
            case Plus:
                return "Plus";
            case Caret:
                return "Caret";
            case Semicolon:
                return "Semicolon";
            case StringLiteral:
                return "StringLiteral";
            case VariableKeyword:
                return "VariableKeyword";
            case BangEqualsOperator:
                return "BangEqualsOperator";
            case ExclamationMark:
                return "ExclamationMark";
            case HyphenArrow:
                return "HyphenArrow";
            case Comma:
                return "Comma";
            case Period:
                return "Period";
            case TrueKeyword:
                return "True";
            case FalseKeyword:
                return "False";
            case Increment:
                return "Increment";
            case Decrement:
                return "Decrement";
            case EqualOrGreaterThanOperator:
                return "EqualOrGreaterThan";
            case EqualOrLessThanOperator:
                return "EqualOrLessThan";
            case Colon:
                return "Colon";
            case None:
                return "None";
            case Unimplemented:
                //default:
                return "Unimplemented";
        }
    }
};
