#pragma once
#include "Ela.hpp"
#include "Statement.hpp"
#include "Token.hpp"

using namespace Expressions;

enum class NonTerminalType {
  Programm,
  ImportStatementList,
  ImportStatement,
  FunctionDefinitinList,
  None,

};

using Symbol = std::variant<NonTerminalType, Token>;

// do not try to format your macros correctly. It's painful to do. Let clang-fmt do it ;-)
// bsp: N(type, T() else T() else)
#define T(type, varName, tokenAction)                                          \
  if (match(type)) {                                                           \
    Token varName = mCurrentToken();                                           \
    { tokenAction }                                                            \
  }
#define NO_T(action)                                                            \
  else{error}
#define N(type, row)                                                           \
  if (top() == type) {                                                         \
    pop();                                                                     \
    row                                                                        \
  }



struct ParserOpts {
  int verbosityLevel;
};

class Parser {
public:
  Parser(std::vector<Token> tokens, ParserOpts opts)
      : mOpts{opts}, mTokens{tokens}, mLogger{opts.verbosityLevel} {};
  [[nodiscard]] Programm parse();

private:
  logger::Logger mLogger;

  std::vector<Token> mTokens;
  ParserOpts mOpts;

  std::size_t mCurrentPos{0};
  Token mCurrentToken();
  void consume();
  bool consume(TokenType type);
  Token consumeOrError(TokenType type);
  bool match(std::initializer_list<TokenType> types);
  void mParserError(TokenType expected, Token found);
  OperatorType mOperatorType(Token t);

  [[nodiscard]] std::vector<Expression> mParseUntil(TokenType t);
  Token previous();
  Token next();

  Expression mTerm();
  Programm mProgramm();

  std::vector<Symbol> stack;

  NonTerminalType top();
  void pop();
  void push(Symbol s);
};
