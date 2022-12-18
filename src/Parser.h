#pragma once

#include <utility>

#include "Ela.hpp"
#include "Statement.h"
#include "Token.h"

using namespace Expressions;

enum class NonTerminalType {
    Programm,
    ImportStatementList,
    ImportStatement,
    FunctionDefinitionList,
    FunctionDefinition,
    Block,
    None,

};

using Symbol = std::variant<NonTerminalType, TokenType>;

// do not try to format your macros correctly. It's painful to do. Let clang-fmt do it ;-)
// bsp: N(type, T() else T() else)
#define T(type, varName, tokenAction)                                          \
  if (match(type)) {                                                           \
    Token varName = mCurrentToken();                                           \
    { tokenAction }                                                            \
  }
#define ERRORINLOOP(type) mParserError(type, mCurrentToken()); return ParserLoopResult::ParserError;

struct ParserOpts {
    int verbosityLevel;
};

enum ParserLoopResult {
    Continue,
    FinishedParsing,
    ParserError
};

class Parser {
public:
    Parser(std::vector<Token> tokens, ParserOpts opts)
            : mOpts{opts}, mTokens{std::move(tokens)}, mLogger{opts.verbosityLevel} {};

    [[nodiscard]] std::optional<Statements::Programm> parse();

private:
    logger::Logger mLogger;

    std::vector<Token> mTokens;
    ParserOpts mOpts;

    std::size_t mCurrentPos{0};

    Token mCurrentToken();

    void consume();

    bool consume(TokenType type);

    Token consumeOrError(TokenType type);

    bool match(TokenType type);

    void mParserError(TokenType expected, Token found);

    ParserLoopResult mParserLoop();

    OperatorType mOperatorType(Token t);

    Token previous();

    Token next();

    std::optional<Statements::Programm> mProgramm();

    std::vector<Symbol> stack;

    Symbol top();

    void pop();

    void push(Symbol s);

    //Node &currentNode;
    Statements::Programm mP;

    static bool is(Symbol s, TokenType t);

    static bool is(Symbol s, NonTerminalType t);

    Statements::FunctionDefinition mFunctionDefinition();
    Statements::ImportStatement mImportStatement();
    TypeExpressions::TypeExpression * mTypeExpression();

    Token matchOrError(TokenType type);

    Statements::BlockStatement mBlockStatement();

    Statements::Statement *mStatement();

    Statements::VariableDefinitionStatement * mVariableDefinition();

    Expression *mExpression();
};
