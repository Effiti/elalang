#pragma once
#include "Ela.hpp"


using std::string;
enum class TokenType {
  None,
  StringLiteral,

  BlockBegin, // done
  BlockEnd,   // done

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
  EqualsOperator,
  EqualOrGreaterThanOperator,
  EqualOrLessThanOperator,
  AssignmentOperator,

  Semicolon,

  VariableKeyword,
  ImportKeyword,

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
