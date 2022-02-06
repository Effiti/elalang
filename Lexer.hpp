#pragma once

#include "Token.hpp"
#include "Ela.hpp"

class Lexer {
  public:
  Lexer(std::string_view source, int verbosity=1) : mStrSource{source}, mLogger(verbosity)  {}
  void consume();
  [[nodiscard]] bool consume(char c);
  constexpr void consumeOrError(char c, std::string_view errorMessage) const
      throw();
  [[nodiscard]] constexpr std::string_view peek(std::size_t len);
  [[nodiscard]] constexpr char peek() const;
  [[nodiscard]] constexpr bool isOverEnd() const;
  [[nodiscard]] constexpr bool check(char c) const;
  [[nodiscard]] std::vector<Token> parseSource();
  void logLexerError(std::string s);

private:
  std::string_view mStrSource;
  std::size_t mCurrentPos{0};
  std::size_t mCurrentLine{1};
  std::size_t mCurrentCol{1};

  std::vector<Token> mTokens;
  void mAddToken(Token token);

  std::string mWordToken();
  std::string mStringLiteralToken();
  std::string mUnescapeStringLiteral(std::string s);

  TokenType mGetWordTokenType();
  TokenType mGetSingleCharTokenType(char c);

  Token mParseWordToken(std::string);
  Token mParseSingleCharToken(char c);
  Token mParseStringLiteralToken(std::string s);
  logger::Logger mLogger;
};
