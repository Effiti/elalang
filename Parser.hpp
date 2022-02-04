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
      : mOpts{opts}, mTokens{tokens} {};
  [[nodiscard]] std::vector<ElaNode> parse();

private:
  std::vector<Token> mTokens;
  ParserOpts mOpts;
};
