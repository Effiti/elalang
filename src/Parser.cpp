#include "Parser.hpp"
#include "Statement.hpp"
#include <algorithm>

void Parser::mParserError(TokenType expected, Token found) {
  std::string file{found.file};
  mLogger.log(logger::messageType::FATAL_ERROR,
              "expected " + humanReadableTokenType(expected) + " but got " +
                  humanReadableTokenType(found.type),
              found.line, file);
}
Programm Parser::parse() {
  return mProgramm();
}



Programm Parser::mProgramm() {
  
  push(1);
  T(TokenType::ImportKeyword, t, )
  mImportStatements();
}

SameTypeNodeList<ImportStatement> mImportStatements() {
  while (consume(TokenType::ImportKeyword))
  {
    
  }
}


void Parser::consume() {
  if (mCurrentPos < mTokens.size())
    mCurrentPos++;
}

bool Parser::consume(TokenType type) {
  if(!mCurrentToken().type != type)
    return false;
  consume();
  return true;

}
Token Parser::mCurrentToken() { return mTokens.at(mCurrentPos); }

bool Parser::match(std::initializer_list<TokenType> types) {
  for(auto type : types)
  {
    if(mCurrentToken().type == type)
      return true;
  }
  return false;
}

Token Parser::next() {
  //FIXME STUB
  return mTokens.at(mCurrentPos+1);
}
Token Parser::previous() {
  //FIXME STUB
  return mTokens.at(mCurrentPos-1);
}

Expression Parser::mTerm() {
  if(mCurrentToken().type == TokenType::NumberLiteral) {
  }
}

