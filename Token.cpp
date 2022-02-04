#include "Token.hpp"

std::string humanReadableTokenType(TokenType t) {
  using enum TokenType;
  switch (t) {

  case None:
    return "None";
  case StringLiteral:
    return "StringLiteral";
  case SubStackBegin:
    return "SubStackBegin";
  case SubStackClose:
    return "SubStackClose";
  case StackEvalQualifier:
    return "StackEvalQualifier";
  case ExplicitConstQualifier:
    return "ExplicitConstQualifier";
  case PlusOperator:
    return "PlusOperator";
  case MinusOperator:
    return "MinusOperator";
  case PowerOperator:
    return "PowerOperator";
  case MultiplicationOperator:
    return "MultiplicationOperator";
  case DivisionOperator:
    return "DivisionOperator";
  case ParenthesesOpen:
    return "ParenthesesOpen";
  case ParenthesesClose:
    return "ParenthesesClose";
  case BracketsOpen:
    return "BracketsOpen";
  case BracketsClose:
    return "BracketsClose";
  case GreaterThanOperator:
    return "GreaterThanOperator";
  case LessThanOperator:
    return "LessThanOperator";
  case VariableKeyword:
    return "VariableKeyword";
  case Identifier:
    return "Identifier";
  case Semicolon:
    return "Semicolon";
  case Unimplemented:
  default:
    return "Unimplemented";
  }
}
