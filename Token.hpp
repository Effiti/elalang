#pragma once
#include "Ela.hpp"


using std::string;
enum class TokenType {
  None,
  StringLiteral,

  SubStackBegin,
  SubStackClose,

  StackEvalQualifier,
  ExplicitConstQualifier,

  PlusOperator,
  MinusOperator,
  PowerOperator,
  MultiplicationOperator,
  DivisionOperator,
  ParenthesesOpen,
  ParenthesesClose,
  BracketsOpen,
  BracketsClose,
  GreaterThanOperator,
  LessThanOperator,

  Semicolon,

  VariableKeyword,

  Identifier,

  Unimplemented,

  EndOfFile

};
std::string humanReadableTokenType(TokenType t);

class Token {
public:
  Token(TokenType t, string l) : type{t}, lexeme{l} {};
  TokenType type;
  string lexeme;
};
