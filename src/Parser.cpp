#include "Parser.hpp"
#include "Statement.hpp"
#include <algorithm>
using enum NonTerminalType;
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
void Parser::mParserLoop(){
  N(
    ImportStatementList,
    T(ImportKeyword, k, )
    NO_T()


  )
}


Programm Parser::mProgramm() {
  
  push(NonTerminalType::ImportStatementList);
  push(NonTerminalType::FunctionDefinitionList);
  while(!stack.size()!=0){
    mParserLoop();
  }
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
  if(mCurrentToken().type == TokenType::EndOfFile)
    return NULLTOK;
  return mTokens.at(mCurrentPos+1);
}
Token Parser::previous() {
  //FIXME STUB
  if(mCurrentPos==0)
    return NULLTOK;
  return mTokens.at(mCurrentPos-1);
}

Expression Parser::mTerm() {
  if(mCurrentToken().type == TokenType::NumberLiteral) {
  }
}

