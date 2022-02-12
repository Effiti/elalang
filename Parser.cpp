#include "Parser.hpp"
#include "ElaNode.hpp"
#include "Token.hpp"

std::vector<ElaNode> Parser::parse() {
  return mParseUntil(TokenType::EndOfFile);
}
std::vector<ElaNode> Parser::mParseUntil(TokenType t) {
  std::vector<ElaNode> nodes;
  // comparing types is enough.
  while (mCurrentToken().type != t) {
  }

  if (t != TokenType::EndOfFile && mCurrentToken().type == TokenType::EndOfFile)
  {
    // TODO: log error "expected.. got EOF
  }
  return nodes;
}
