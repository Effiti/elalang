#include "Parser.h"
#include "Statement.h"
#include "TypeExpression.h"
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

std::optional<Statements::Programm> Parser::parse() {
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
    N(ImportStatement,
    {
       T(TokenType::ImportKeyword, k,
         {
             Statements::ImportStatement statement = mImportStatement();
             {{ mP.importStatements.subNodes.push_back(statement); }}
         }
       )
        else {
            mParserError(TokenType::ImportKeyword, mCurrentToken());
            return ParserLoopResult::ParserError;;
        }
    }
    )
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
                    mP.functionDefinitions.subNodes.push_back(mFunctionDefinition());
                })
                else{
                    ERRORINLOOP(TokenType::FunctionKeyword);
                }
            }
    )
    if (

            is(top(), mCurrentToken()

                    .type)) {
        pop();

    }
    return
            ParserLoopResult::Continue;

}


std::optional<Statements::Programm> Parser::mProgramm() {
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
    return mP;
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
    Token t = mCurrentToken();
    if (!consume(type)) {
        mParserError(type, mCurrentToken());
    }
    return  t;
}

Token Parser::matchOrError(TokenType type) {
    if (!match(type)) {
        mParserError(type, mCurrentToken());
    }
    return mCurrentToken();
}

OperatorType Parser::mOperatorType(Token t) {
    switch (t.type) {
        case TokenType::DivisionOperator:
            return OperatorType::Division;
        case TokenType::Plus:
            return OperatorType::Plus;
        case TokenType::Hyphen:
            return OperatorType::Minus;
        case TokenType::Asterisk:
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
    if (stack.empty())
        return TokenType::None;
    return stack.at(stack.size() - 1);
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

Statements::ImportStatement Parser::mImportStatement() {
    consumeOrError(TokenType::ImportKeyword);
    // nice and concise!
    auto mod = consumeOrError(TokenType::StringLiteral);
    return {mod.value};
}

Statements::FunctionDefinition Parser::mFunctionDefinition() {
    consumeOrError(TokenType::FunctionKeyword);
    auto name = consumeOrError(TokenType::Identifier).value;
    // parse parameterList
    /*
     * parameterList -> parameter , parameterList | EPSILON
     * parameter -> Identifier : TypeExpression
     */
    consumeOrError(TokenType::LParen);
    if (!match(TokenType::RParen)) {
        do {


        } while (match(TokenType::Comma));

    }
    auto returnType = new TypeExpressions::SimpleType{TypeExpressions::BaseType::Integer};

    return Statements::FunctionDefinition{
            returnType, "test"sv,
            SameTypeNodeList<Statements::Parameter>{}};


}
