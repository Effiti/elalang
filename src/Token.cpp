#include "Token.hpp"

std::string humanReadableTokenType(TokenType type) {
  using enum TokenType;
  switch (type) {
  case AssignmentOperator:
    return "AssignmentOperator";
  case BlockBegin:
    return "BlockBegin";
  case BlockEnd:
    return "BlockEnd";
  case BracketsClose:
    return "BracketsClose";
  case BracketsOpen:
    return "BracketsOpen";
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
    return "FunctionKeyWord";
  case Identifier:
    return "Identifier";
  case ImportKeyword:
    return "ImportKeyword";
  case LessThanOperator:
    return "LessThanOperator";
  case MinusOperator:
    return "MinusOperator";
  case MultiplicationOperator:
    return "MultiplicationOperator";
  case NumberLiteral:
    return "NumberLiteral";
  case ParenthesesClose:
    return "ParenthesesClose";
  case ParenthesesOpen:
    return "ParenthesesOpen";
  case PlusOperator:
    return "PlusOperator";
  case PowerOperator:
    return "PowerOperator";
  case Semicolon:
    return "Semicolon";
  case StringLiteral:
    return "StringLiteral";
  case VariableKeyword:
    return "VariableKeyword";
  case BangEqualsOperator:
    return "BangEqualsOperator";
  case BangOperator:
    return "BangOperator";

  case None:
    return "None";
  case Unimplemented:
  default:
    return "Unimplemented";
  }
}
