#pragma once

#include "Ela.hpp"
#include "ElaNode.hpp"
#include "Token.hpp"

struct ParserOpts {
  int verbosityLevel;
};

class Parser {
public:
  Parser(std::vector<Token> tokens, ParserOpts opts)
      : mOpts{opts}, mTokens{tokens}, mLogger{opts.verbosityLevel} {};
  [[nodiscard]] std::vector<ElaNode> parse();

private:
  logger::Logger mLogger;

  std::vector<Token> mTokens;
  ParserOpts mOpts;

  std::size_t mCurrentPos{0};
  Token mCurrentToken();
  void consume();
  bool consume(TokenType type);
  void mParserError(TokenType expected, Token found);

  [[nodiscard]] std::vector<ElaNode> mParseUntil(TokenType t);
};
