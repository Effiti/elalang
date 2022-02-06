#include "Lexer.hpp"
#include "Ela.hpp"
#include "Token.hpp"

void Lexer::logLexerError(std::string s) {
  mLogger.log(logger::messageTypes::FATAL_ERROR, s, mCurrentLine, __FILE__);
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
  mLogger.log(logger::messageTypes::DEBUG, std::string("consuming: ") + peek(),
              HERE());
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

// FIX: this does not work...
std::string Lexer::mStringLiteralToken() {
  //consume first '"'
  consume();
  std::string value;
  bool stringEnded{false};
  char buf;
  while(!stringEnded && !isOverEnd()) {
    buf = peek();
    if(peek() == '\\') {
      value += peek();
      consume();
      value += peek();
      buf = peek();
      consume();
    }
    if(buf == '"') {
      stringEnded = true;
      consume();
    }
    else{
      value += peek();
      consume();
    }
  }
  if(isOverEnd() && !stringEnded){
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
      mLogger.log(logger::messageTypes::DEBUG, "parsing string Literal!", HERE());
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

  return mTokens;
}
