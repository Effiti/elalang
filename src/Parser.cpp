#include "Parser.hpp"
#include "Statement.hpp"
#include <algorithm>

using
enum NonTerminalType;

void Parser::mParserError(TokenType expected, Token found) {
    std::string file{found.file};
    mLogger.log(logger::messageType::FATAL_ERROR,
                "expected " + humanReadableTokenType(expected) + " but got " +
                humanReadableTokenType(found.type),
                found.line, file);
}

std::optional<Statement::Programm> Parser::parse() {
    return mProgramm();
}

ParserLoopResult Parser::mParserLoop() {
    N(
            ImportStatementList,
            {
                T(TokenType::ImportKeyword, k,
                  {
                          push(ImportStatementList);
                          push(ImportStatement);
                  }
                )
                else T(TokenType::FunctionKeyword, k, {})
                {
                }
            }
    );
    N(
            FunctionDefinitionList,
            {
                T(TokenType::FunctionKeyword, k,
                  push(FunctionDefinition);
                )
            }
    );
    N(
            ImportStatement,
            {
                T(TokenType::ImportKeyword, k,
                  {
                          consume();
                          push(TokenType::Semicolon);
                          consumeOrError(TokenType::StringLiteral);
                          if(match(TokenType::StringLiteral)){
                              mP.importStatements.subNodes.push_back(Statement::ImportStatement(mCurrentToken().value));
                          }
                          else{
                              mParserError(TokenType::StringLiteral, mCurrentToken());
                          }
                  }
                )
                else {
                    //UNREACHABLE
                    ERRORINLOOP(TokenType::ImportKeyword);
                }
            }
    );
    N(
            FunctionDefinitionList,
            {
                T(TokenType::FunctionKeyword, k,
                  {
                          push(FunctionDefinitionList);
                          push(FunctionDefinition);
                  }
                )
                else T(TokenType::EndOfFile, k,
                       {
                            consume();
                            return ParserLoopResult::FinishedParsing;
                       }
                )
                else {
                    ERRORINLOOP(TokenType::FunctionKeyword)
                }
            }

    )
    N(
            FunctionDefinition,
            {
                T(TokenType::FunctionKeyword, k, {
                        T(TokenType::Identifier, ident, {

                        })
                        else{
                            ERRORINLOOP(TokenType::Identifier);
                        }
                })
                else{
                    ERRORINLOOP(TokenType::FunctionKeyword);
                }
            }
    )
    if (is(top(), mCurrentToken().type)){
        pop();
    }
    return ParserLoopResult::Continue;

}


std::optional<Statement::Programm> Parser::mProgramm() {
    push(NonTerminalType::FunctionDefinitionList);
    push(NonTerminalType::ImportStatementList);
    while (!stack.empty()) {
        switch (mParserLoop()) {
            case ParserLoopResult::Continue:
                continue;
            case ParserLoopResult::FinishedParsing:
                return mP;
            case ParserLoopResult::ParserError:
                throw std::runtime_error("");


        };
    }
    return std::nullopt;
}

void Parser::consume() {
    if (mCurrentPos < mTokens.size())
        mCurrentPos++;
}

bool Parser::consume(TokenType type) {
    if (mCurrentToken().type != type)
        return false;
    consume();
    return true;

}

Token Parser::mCurrentToken() { return mTokens.at(mCurrentPos); }


Token Parser::next() {
    //FIXME STUB
    if (mCurrentToken().type == TokenType::EndOfFile)
        return NULLTOK;
    return mTokens.at(mCurrentPos + 1);
}

Token Parser::previous() {
    //FIXME STUB
    if (mCurrentPos == 0)
        return NULLTOK;
    return mTokens.at(mCurrentPos - 1);
}


bool Parser::match(TokenType type) {
    return mCurrentToken().type == type;

}

Token Parser::consumeOrError(TokenType type) {
    if (!match(type)) {
        mParserError(type, mCurrentToken());
    }
    return Token{0, 0, TokenType::Unimplemented, ""sv};
}

OperatorType Parser::mOperatorType(Token t) {
    switch (t.type) {
        case TokenType::DivisionOperator:
            return OperatorType::Division;
        case TokenType::PlusOperator:
            return OperatorType::Plus;
        case TokenType::MinusOperator:
            return OperatorType::Minus;
        case TokenType::MultiplicationOperator:
            return OperatorType::Multiplication;
        default:
            return OperatorType::None;
    }
}

bool Parser::is(Symbol s, TokenType t) {
    if (std::holds_alternative<NonTerminalType>(s))
        return false;
    if (std::get<TokenType>(s) == t)
        return true;
    else
        return false;
}

Symbol Parser::top() {
    if(stack.empty())
        return TokenType::None;
    return stack.at(stack.size()-1);
}

bool Parser::is(Symbol s, NonTerminalType t) {
    if (std::holds_alternative<TokenType>(s))
        return false;
    if (std::get<NonTerminalType>(s) == t)
        return true;
    else
        return false;
}

void Parser::push(Symbol s) {
    stack.push_back(s);
}

void Parser::pop() {
    stack.pop_back();
}
