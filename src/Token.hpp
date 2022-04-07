#pragma once
#include "Ela.hpp"

using std::string;
using namespace std::string_view_literals;
enum class TokenType {
  None,
  NumberLiteral,
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
std::string humanReadableTokenType(TokenType type);

class Token {
public:
  Token(std::size_t l, std::size_t c, TokenType t, string v)
      : line{l}, col{c}, type{t}, value{v}, file{"main.ela"sv} {};
  std::size_t line;
  std::size_t col;
  TokenType type;
  // TODO: change value's type to std::string_view, imlement "file" field in
  // other files
  string value;
  std::string_view file;
};
