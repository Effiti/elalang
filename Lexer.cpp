#include "Lexer.hpp"
#include "Ela.hpp"
#include "Token.hpp"

void Lexer::logLexerError(std::string s) {
  logger::Logger l;
  l.log(logger::messageTypes::FATAL_ERROR, s, mCurrentLine, __FILE__);
}

[[nodiscard]] bool Lexer::consume(char c) {
  if (mStrSource.at(mCurrentPos) == c) {
    mCurrentPos++;

    if (c == '\n') {
      mCurrentLine++;
      mCurrentCol = 0;
    } else
      mCurrentCol++;
    return true;
  }
  return false;
}

void Lexer::mAddToken(Token token) { mTokens.push_back(token); }

void Lexer::consume() { mCurrentPos++; }

// WARNING:THIS DOES NOT HANDLE isOverEnd() !!!!!
constexpr char Lexer::peek() const { return mStrSource.at(mCurrentPos); }

constexpr std::string_view Lexer::peek(std::size_t len) {
  return mStrSource.substr(mCurrentPos - 1, len);
}

constexpr bool Lexer::isOverEnd() const {
  return (mCurrentPos >= mStrSource.length());
}

constexpr bool Lexer::check(char c) const { return peek() == c; }

std::string Lexer::mWordToken() {
  std::string lexeme;
  while (!(std::isspace(peek())) && (std::isalnum(peek()) || peek() == '_')) {
    lexeme += peek();
    consume();
  }
  if (std::isspace(peek()))
    consume();
  return lexeme;
}

Token Lexer::mParseWordToken(std::string word) {
  if (word == "var") {
    return Token{TokenType::VariableKeyword, word};
  }
  return Token{TokenType::Identifier, word};
}

TokenType Lexer::mGetSingleCharTokenType(char c) {
  using enum TokenType;
  switch (c) {
  case 'c':
    return ExplicitConstQualifier;
  case '@':
    return StackEvalQualifier;
  case '-':
    return MinusOperator;
  case '*':
    return MultiplicationOperator;
  case '/':
    return DivisionOperator;
  case '^':
    return PowerOperator;
  case '(':
    return ParenthesesOpen;
  case ')':
    return ParenthesesClose;
  case '[':
    return BracketsOpen;
  case ']':
    return BracketsClose;
  case '{':
    return SubStackBegin;
  case '}':
    return SubStackClose;
  case '>':
    return GreaterThanOperator;
  case '<':
    return LessThanOperator;
  case ';':
    return Semicolon;
  default:
    return None;
  }
}
// TODO: Implement
std::string Lexer::mUnescapeStringLiteral(std::string s) { return s; }

std::string Lexer::mStringLiteralToken() {
  // removes first '"'
  consume();
  char currentChar = peek();
  std::string value;
  while (currentChar != '"' && !isOverEnd()) {
    currentChar = peek();
    if (currentChar == '\\') {
      value += peek();
      consume();
      if (!(mCurrentPos >= mStrSource.length())) {
        value += peek();
        consume();
      } else {
        logLexerError("unexpected EOF in parsing string literal!");
        break;
      }
    } else if (currentChar == '"' &&
               !(mCurrentPos == mStrSource.length() - 1)) {
      consume();
      break;
    } else if (currentChar == '"') {
      return value;
    } else {
      value += currentChar;
      consume();
    }
  }
  if (isOverEnd()) {
    logLexerError("unexpected EOF in parsing string literal!");
  }
  return value;
}
Token Lexer::mParseStringLiteralToken(std::string s) {
  return Token{TokenType::StringLiteral, s};
}

Token Lexer::mParseSingleCharToken(char c) {
  return Token{mGetSingleCharTokenType(c), std::string(1, c)};
}

std::vector<Token> Lexer::parseSource() {
  char currentChar = peek();
  while (!isOverEnd()) {
    currentChar = peek();
    if (std::isspace(currentChar)) {
      consume();
      currentChar = peek();
      continue;
    }

    switch (currentChar) {
    case 'c':
    case '@':
    case '+':
    case '-':
    case '*':
    case '[':
    case '/':
    case '(':
    case '^':
    case ']':
    case ')':
    case '{':
    case '}':
    case ';':
      mAddToken(mParseSingleCharToken(currentChar));
      consume();
      break;
    case '"':
      mAddToken(mParseStringLiteralToken(mStringLiteralToken()));
      break;
    default:
      mAddToken(mParseWordToken(mWordToken()));
      break;
    }

    if (!isOverEnd())
      currentChar = peek();
    if (mCurrentPos == mStrSource.length() - 1)
      break;
  }

  return mTokens;
}
