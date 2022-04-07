#include "Ela.hpp"
#include "Parser.hpp"
#include "ElaNode.hpp"
#include "Token.hpp"
void Parser::mParserError(TokenType expected, Token found){
  std::string file{found.file};
    mLogger.log(logger::messageType::FATAL_ERROR,
                "expected " + humanReadableTokenType(expected) + " but got " +
                    humanReadableTokenType(found.type),
                 found.line, file);
}
std::vector<ElaNode> Parser::parse() {
  return mParseUntil(TokenType::EndOfFile);
}
std::vector<ElaNode> Parser::mParseUntil(TokenType t) {
  ElaNode current;
  std::vector<ElaNode> nodes;
  // comparing types is enough.
  while (mCurrentToken().type != t) {
    if (mCurrentToken().type == TokenType::BlockBegin) {
      consume();
      nodes.push_back(BlockNode{mParseUntil(TokenType::BlockEnd)});
    } else {

      consume();
      //break;
    }
  }

  if (t != TokenType::EndOfFile &&
      mCurrentToken().type == TokenType::EndOfFile) {

  }
  return nodes;
}
void Parser::consume() {
  if (mCurrentPos < mTokens.size())
    mCurrentPos++;
}
Token Parser::mCurrentToken() { return mTokens.at(mCurrentPos); }