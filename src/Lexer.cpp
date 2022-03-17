#include "Lexer.hpp"
#include "Ela.hpp"
#include "Token.hpp"
#include "cmdlib/logging.hpp"
#include <string>



void Lexer::logLexerError(std::string s) {
  mLogger.log(logger::messageType::FATAL_ERROR, s, mCurrentLine, __FILE__);
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

void Lexer::consume() {
  mCurrentPos++;
}

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
  if (word == "import") {
    return Token{TokenType::ImportKeyword, word};
  }
  return Token{TokenType::Identifier, word};
}

TokenType Lexer::mGetSingleCharTokenType(char c) {
  using enum TokenType;
  switch (c) {
  case 'c':
    return ExplicitConstQualifier;
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
    return BlockBegin;
  case '}':
    return BlockEnd;
  case '>':
    return GreaterThanOperator;
  case '<':
    return LessThanOperator;
  case ';':
    return Semicolon;
  case '=':
    return EqualsOperator;
  default:
    return None;
  }
}
// TODO: Implement
std::string Lexer::mUnescapeStringLiteral(std::string s) { return s; }

std::string Lexer::mStringLiteralToken() {
  // consume first '"'
  consume();
  std::string value;
  bool stringEnded{false};
  char buf;
  while (!stringEnded && !isOverEnd()) {
    buf = peek();
    if (buf == '\\') {
      value += peek();
      consume();
      value += peek();
      buf = peek();
      consume();
    }
    if (buf == '"') {
      stringEnded = true;
      consume();
    } else {
      value += peek();
      consume();
    }
  }
  if (isOverEnd() && !stringEnded) {
    logLexerError("unexpected EOF in parsing string literal token!");
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
      if(isOverEnd())
        break;
      currentChar = peek();
      continue;
    }

    switch (currentChar) {
    case '/':
      if (mStrSource.length() > mCurrentPos - 1 &&
          mStrSource.at(mCurrentPos + 1) == '/') {
        while (mCurrentPos < mStrSource.length() && peek() != '\n' ) {
          consume();
        }
        break;
      }
      mAddToken(mParseSingleCharToken(currentChar));
      consume();
      break;
    case '=':
      if (mStrSource.length() > mCurrentPos - 1 &&
          mStrSource.at(mCurrentPos + 1) == '=') {
        mAddToken(Token{TokenType::AssignmentOperator, "=="});
        consume();
        consume();
        break;
      }
    case 'c':
    case '@':
    case '+':
    case '-':
    case '*':
    case '[':
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
    else
      break;
  }
  mAddToken(Token{TokenType::EndOfFile, ""});

  return mTokens;
}
