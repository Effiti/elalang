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
    if (match(TokenType::ImportKeyword)) {

    }
    if (is(top(), ImportStatementList)) {
        pop();
        {
            if (match(TokenType::ImportKeyword)) {
                {
                    push(ImportStatementList);
                    push(ImportStatement);
                }
            }
        }
    };
    if (is(top(), ImportStatement)) {
        pop();
        {
            if (match(TokenType::ImportKeyword)) {
                Statements::ImportStatement statement = mImportStatement();
                mP.importStatements.push_back(statement);
            } else {
                mParserError(TokenType::ImportKeyword, mCurrentToken());
                return ParserLoopResult::ParserError;;
            }
        }
    }
    if (is(top(), FunctionDefinitionList)) {
        pop();
        {
            if (match(TokenType::FunctionKeyword)) {
                push(FunctionDefinitionList);
                push(FunctionDefinition);
            } else if (match(TokenType::EndOfFile)) {
                consume();
                return ParserLoopResult::FinishedParsing;
            } else {
                mParserError(TokenType::FunctionKeyword, mCurrentToken());
                return ParserLoopResult::ParserError;
            }
        }
    }
    if (is(top(), FunctionDefinition)) {
        pop();
        {
            if (match(TokenType::FunctionKeyword)) {
                mP.functionDefinitions.push_back(mFunctionDefinition());
            } else {
                mParserError(TokenType::FunctionKeyword, mCurrentToken());
                return ParserLoopResult::ParserError;;
            }
        }
    }
    if (is(top(), mCurrentToken().type)) {
        pop();
    }
    return ParserLoopResult::Continue;

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
    return t;
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
    consumeOrError(TokenType::Semicolon);
    return Statements::ImportStatement{mod.value};
}

Statements::FunctionDefinition Parser::mFunctionDefinition() {
    consumeOrError(TokenType::FunctionKeyword);
    std::string name = consumeOrError(TokenType::Identifier).value;
    // parse parameterList
    /*
     * parameterList -> parameter , parameterList | EPSILON
     * parameter -> Identifier : TypeExpression
     */
    consumeOrError(TokenType::LParen);
    vector<Statements::Parameter> params;
    if (!match(TokenType::RParen)) {
        std::string paramName;
        do {
            paramName = consumeOrError(TokenType::Identifier).value;
            consumeOrError(TokenType::Colon);
            params.emplace_back(mTypeExpression(), paramName);


        } while (consume(TokenType::Comma));

    }
    consumeOrError(TokenType::RParen);
    TypeExpressions::TypeExpression *returnType;
    if (!consume(TokenType::HyphenArrow)) {
        returnType = new TypeExpressions::SimpleType{TypeExpressions::Void};
    } else {
        returnType = mTypeExpression();
    }
    Statements::BlockStatement block = mBlockStatement();

    return Statements::FunctionDefinition{
            returnType, name,
            params, block};
}

TypeExpressions::TypeExpression *Parser::mTypeExpression() {
    const std::string baseTypeName = consumeOrError(TokenType::Identifier).value;
    const std::optional<TypeExpressions::BaseType> base = TypeExpressions::getBaseType(baseTypeName);
    std::variant<const std::string, TypeExpressions::BaseType> baseVar = baseTypeName;
    if (base.has_value()) {
        baseVar = base.value();

    }
    auto *simpleType = new TypeExpressions::SimpleType{baseVar};
    if (!consume(TokenType::LBracket)) {
        return simpleType;
    }
    vector<TypeExpressions::TypeExpression *> args;
    do {
        args.push_back(mTypeExpression());

    } while (consume(TokenType::Comma));
    consumeOrError(TokenType::RBracket);

    return new TypeExpressions::TypeTemplateExpression(*simpleType, args);
}

Statements::BlockStatement Parser::mBlockStatement() {
    consumeOrError(TokenType::LCurly);
    std::vector<Statements::Statement *> statements;
    // TODO: emit a warning or *mark the AST-Node* if the block is empty
    //       this also includes blocks which only contain an arbitrary amount of Semicolons:
    //       {;;;;;;;;} //is semantically empty!
    //       add an isEmpty-member to Statement?
    // we allow empty blocks, so we do not need a do{ }while loop
    while (!match(TokenType::RCurly)) {
        statements.push_back(mStatement());
    }
    consumeOrError(TokenType::RCurly);
    return {statements};
}

Statements::Statement *Parser::mStatement() {
    //if(match(TokenType::VariableKeyword))
    return mVariableDefinition();


}

Statements::VariableDefinitionStatement *Parser::mVariableDefinition() {
    // TODO
    consumeOrError(TokenType::VariableKeyword);
    std::string name = consumeOrError(TokenType::Identifier).value;
    consumeOrError(TokenType::Colon);
    TypeExpressions::TypeExpression *type = mTypeExpression();
    consumeOrError(TokenType::Semicolon);
    return new Statements::VariableDefinitionStatement{name, type, new Expressions::IntegerLiteral{12}};
}

Expressions::Expression *Parser::mExpression() {
    return Expressions::IntegerLiteral(12);


}