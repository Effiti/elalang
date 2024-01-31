#include "Parser.h"

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "Expression.h"
#include "Statement.h"
#include "TypeExpression.h"
namespace Ela {

using enum NonTerminalType;
using std::unique_ptr, std::make_unique, std::shared_ptr, std::make_shared;
std::string humanReadableNonTerminalType(NonTerminalType type) {
  switch (type) {
    case NonTerminalType::Block:
      return "Block";
    case NonTerminalType::Expression:
      return "Expression";
    case NonTerminalType::FunctionDefinition:
      return "FunctionDefinition";
    case NonTerminalType::FunctionDefinitionList:
      return "FunctionDefinition[]";
    case NonTerminalType::ImportStatement:
      return "ImportStatement";
    case NonTerminalType::None:
      return "None";
    case NonTerminalType::PrimaryExpression:
      return "PrimaryExpression";
    case NonTerminalType::Program:
      return "Program";
    default:
      return "Unimplemented";
  }
}

void Parser::mParserError(Symbol expected, Token found) {
  std::string file{found.file};
  std::cerr << colors["red"] << "[ERROR] " << found.file << ":" << found.line
            << ":" << found.col << " expected ";
  if (std::holds_alternative<TokenType>(expected))
    std::cerr << humanReadableTokenType(std::get<TokenType>(expected));

  else
    std::cerr << humanReadableNonTerminalType(
        std::get<NonTerminalType>(expected));
  std::cerr << " but found " << humanReadableTokenType(found.type) << std::endl;
  throw std::runtime_error("parser error");
}

std::optional<Statements::Program> Parser::parse() { return mProgram(); }

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
        return ParserLoopResult::ParserError;
        ;
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
        return ParserLoopResult::ParserError;
        ;
      }
    }
  }
  if (is(top(), mCurrentToken().type)) {
    pop();
  }
  return ParserLoopResult::Continue;
}

std::optional<Statements::Program> Parser::mProgram() {
  push(NonTerminalType::FunctionDefinitionList);
  push(NonTerminalType::ImportStatementList);
  while (!stack.empty()) {
    switch (mParserLoop()) {
      case ParserLoopResult::Continue:
        continue;
      case ParserLoopResult::FinishedParsing:
        return std::move(mP);
      case ParserLoopResult::ParserError:
        throw std::runtime_error("");
    };
  }
  return std::move(mP);
}

void Parser::consume() {
  if (mCurrentPos < mTokens.size()) mCurrentPos++;
}

bool Parser::consume(TokenType type) {
  if (mCurrentToken().type != type) return false;
  consume();
  return true;
}

Token Parser::mCurrentToken() { return mTokens.at(mCurrentPos); }

Token Parser::next() {
  // FIXME STUB
  if (mCurrentToken().type == TokenType::EndOfFile) return NULLTOK;
  return mTokens.at(mCurrentPos + 1);
}

Token Parser::previous() {
  // FIXME STUB
  if (mCurrentPos == 0) return NULLTOK;
  return mTokens.at(mCurrentPos - 1);
}

bool Parser::match(TokenType type) { return mCurrentToken().type == type; }

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

BinaryOperatorType Parser::getBinaryOperatorType(const Token &t) {
  switch (t.type) {
    case TokenType::DivisionOperator:
      return BinaryOperatorType::Division;
    case TokenType::Plus:
      return BinaryOperatorType::Plus;
    case TokenType::Hyphen:
      return BinaryOperatorType::Minus;
    case TokenType::Asterisk:
      return BinaryOperatorType::Multiplication;
    case TokenType::EqualsOperator:
      return BinaryOperatorType::Equal;
    case TokenType::BangEqualsOperator:
      return BinaryOperatorType::UnEqual;
    case TokenType::GreaterThanOperator:
      return BinaryOperatorType::Greater;
    case TokenType::LessThanOperator:
      return BinaryOperatorType::Less;
    case TokenType::EqualOrGreaterThanOperator:
      return BinaryOperatorType::GreaterEqual;
    case TokenType::EqualOrLessThanOperator:
      return BinaryOperatorType::LessEqual;
    default:
      return BinaryOperatorType::None;
  }
}

bool Parser::is(Symbol s, TokenType t) {
  if (std::holds_alternative<NonTerminalType>(s)) return false;
  if (std::get<TokenType>(s) == t)
    return true;
  else
    return false;
}

Symbol Parser::top() {
  if (stack.empty()) return TokenType::None;
  return stack.at(stack.size() - 1);
}

bool Parser::is(Symbol s, NonTerminalType t) {
  if (std::holds_alternative<TokenType>(s)) return false;
  if (std::get<NonTerminalType>(s) == t)
    return true;
  else
    return false;
}

void Parser::push(Symbol s) { stack.push_back(s); }

void Parser::pop() { stack.pop_back(); }

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
      params.emplace_back(std::move(mTypeExpression()), paramName);
    } while (consume(TokenType::Comma));
  }
  consumeOrError(TokenType::RParen);
  shared_ptr<TypeExpressions::TypeExpression> returnType;
  if (consume(TokenType::HyphenArrow)) {
    returnType = mTypeExpression();
  } else {
    returnType =
        make_unique<TypeExpressions::SimpleType>(TypeExpressions::Void);
  }
  auto block = *std::move(mBlockStatement());

  return Statements::FunctionDefinition{std::move(returnType), name,
                                        std::move(params), std::move(block)};
}

shared_ptr<TypeExpressions::TypeExpression> Parser::mTypeExpression() {
  if (consume(Lexing::TokenType::LParen)) {
    vector<std::shared_ptr<TypeExpressions::TypeExpression>> types;
    do {
      types.push_back(mTypeExpression());
    } while (consume(Lexing::TokenType::Comma));
    return std::make_shared<TypeExpressions::TypeExpression>(
        TypeExpressions::TupleTypeExpression(types));
  }
  if (!match(Lexing::TokenType::Identifier) &&
      !match(Lexing::TokenType::LBracket))
    mParserError(Lexing::TokenType::Identifier, mCurrentToken());
  const std::string baseTypeName =
      match(Lexing::TokenType::Identifier)
          ? consumeOrError(TokenType::Identifier).value
          : "array";

  const std::optional<TypeExpressions::BaseType> base =
      TypeExpressions::getBaseType(baseTypeName);
  std::variant<const std::string, TypeExpressions::BaseType> baseVar =
      baseTypeName;
  if (base.has_value()) {
    //    std::cout << TypeExpressions::to_string(*base) << std::endl;
    baseVar = base.value();
  }
  auto simpleType = TypeExpressions::SimpleType(baseVar);

  if (!consume(TokenType::LBracket)) {
    return make_shared<TypeExpressions::SimpleType>(simpleType);
  }
  vector<std::variant<shared_ptr<TypeExpressions::TypeExpression>, int>> args;
  do {
    if (match(Lexing::TokenType::NumberLiteral))
      args.push_back(
          std::stoi(consumeOrError(Lexing::TokenType::NumberLiteral).value));
    else
      args.push_back(mTypeExpression());
  } while (consume(TokenType::Comma));
  consumeOrError(TokenType::RBracket);

  return make_shared<TypeExpressions::TypeTemplateExpression>(simpleType,
                                                              std::move(args));
}

shared_ptr<Statements::BlockStatement> Parser::mBlockStatement() {
  consumeOrError(TokenType::LCurly);
  std::vector<shared_ptr<Statements::Statement>> statements;
  // TODO: emit a warning or *mark the AST-Node* if the block is empty
  //       this also includes blocks which only contain an arbitrary amount of
  //       Semicolons:
  //       {;;;;;;;;} //is semantically empty!
  //       add an isEmpty-member to Statement?
  // we allow empty blocks, so we do not need a do{ }while loop
  while (!match(TokenType::RCurly)) {
    statements.push_back(std::move(mStatement()));
  }
  consumeOrError(TokenType::RCurly);
  return make_shared<Statements::BlockStatement>(std::move(statements));
}

shared_ptr<Statements::Statement> Parser::mStatement() {
  if (match(TokenType::VariableKeyword))
    return std::move(mVariableDefinition());
  else if (match(TokenType::IfKeyword))
    return std::move(mIfStatement());
  else if (match(TokenType::LCurly))
    return std::move(mBlockStatement());
  else if (match(TokenType::ElseKeyword))
    return std::move(mElseStatement());
  else if (match(TokenType::ReturnKeyword))
    return std::move(mReturnStatement());
  return std::move(mExpressionStatement());
}

shared_ptr<Statements::ReturnStatement> Parser::mReturnStatement() {
  consumeOrError(TokenType::ReturnKeyword);
  if (consume(TokenType::Semicolon))
    // returning in void functions is the same thing as returning null.
    return make_shared<Statements::ReturnStatement>(
        make_shared<Expressions::NullExpression>());
  auto expr = mExpression();
  consumeOrError(TokenType::Semicolon);
  return make_shared<Statements::ReturnStatement>(std::move(expr));
}

shared_ptr<Statements::ElseStatement> Parser::mElseStatement() {
  consumeOrError(TokenType::ElseKeyword);
  return make_shared<Statements::ElseStatement>(std::move(mStatement()));
}

shared_ptr<Statements::ExpressionStatement> Parser::mExpressionStatement() {
  auto statement = std::make_shared<Statements::ExpressionStatement>(
      std::move(mExpression()));
  consumeOrError(TokenType::Semicolon);
  return statement;
}

shared_ptr<Statements::IfStatement> Parser::mIfStatement() {
  consumeOrError(TokenType::IfKeyword);
  consumeOrError(TokenType::LParen);
  shared_ptr<Expressions::Expression> condition = mExpression();
  consumeOrError(TokenType::RParen);
  shared_ptr<Statements::Statement> statement = mStatement();
  return std::make_unique<Statements::IfStatement>(std::move(condition),
                                                   std::move(statement));
}

shared_ptr<Statements::VariableDefinitionStatement>
Parser::mVariableDefinition() {
  // TODO
  consumeOrError(TokenType::VariableKeyword);
  std::string name = consumeOrError(TokenType::Identifier).value;
  consumeOrError(TokenType::Colon);
  auto type = match(TokenType::AssignmentOperator)
                  ? std::make_shared<TypeExpressions::SimpleType>(
                        TypeExpressions::Infer)
                  : mTypeExpression();
  if (match(TokenType::Semicolon)) {
    consumeOrError(TokenType::Semicolon);
    return make_unique<Statements::VariableDefinitionStatement>(
        name, std::move(type), make_unique<Expressions::NullExpression>());
  }
  consumeOrError(TokenType::AssignmentOperator);
  auto expr = mExpression();
  consumeOrError(TokenType::Semicolon);
  return make_unique<Statements::VariableDefinitionStatement>(
      name, std::move(type), std::move(expr));
}

shared_ptr<Expressions::Expression> Parser::mExpression() {
  // TODO Refactor
  shared_ptr<Expressions::Expression> expr = mPrimaryExpression();
  BinaryOperatorType type = BinaryOperatorType::None;
  using enum BinaryOperatorType;
  while ((type = getBinaryOperatorType(mCurrentToken())) !=
         BinaryOperatorType::None) {
    consume();
    switch (type) {
      case Multiplication:
      case Division:
        expr = make_unique<Expressions::Binary>(std::move(expr), type,
                                                mPrimaryExpression());
        break;
      case Plus:
      case Minus:
        expr = make_unique<Expressions::Binary>(std::move(expr), type,
                                                mSecondaryExpression());
        break;
      case RightShift:
      case LeftShift:
      case Equal:
      case UnEqual:
      case Greater:
      case Less:
      case GreaterEqual:
      case LessEqual:
        expr = make_unique<Expressions::Binary>(std::move(expr), type,
                                                mExpression());
        break;
      case None:
      case MemberAccess:
        throw std::runtime_error("parser error");
    }
  }
  return expr;
}

shared_ptr<Expressions::Expression> Parser::mPrimaryExpression() {
  if (match(TokenType::NumberLiteral)) {
    return make_unique<Expressions::IntegerLiteral>(
        std::stoi(consumeOrError(TokenType::NumberLiteral).value));
  }
  if (consume(Lexing::TokenType::LBracket)) {
    vector<shared_ptr<Expressions::Expression>> exprs;
    if (consume(Lexing::TokenType::RBracket))
      return make_shared<Expressions::ArrayLiteral>(
          std::vector<std::shared_ptr<Expressions::Expression>>{});
    while (!consume(TokenType::RBracket)) {
      exprs.push_back(mExpression());
      if (!match(Lexing::TokenType::RBracket))
        consumeOrError(Lexing::TokenType::Comma);
      else
        // allow trailing comma
        consume(TokenType::Comma);
    }
    return make_shared<Expressions::ArrayLiteral>(exprs);
  }
  if (match(TokenType::Identifier)) {
    shared_ptr<Expressions::Expression> expr;

    if (next().type == TokenType::LParen) {
      auto call = mFunctionCall();
      expr = std::move(call);
    } else {
      auto var = make_unique<Expressions::VariableReference>(
          consumeOrError(TokenType::Identifier).value);
      expr = std::move(var);
    }
    // TODO
    while (consume(TokenType::Period)) {
      if (next().type == TokenType::LParen) {
        auto call = mFunctionCall();
        expr = make_unique<Expressions::Binary>(
            std::move(expr), BinaryOperatorType::MemberAccess, std::move(call));
      } else {
        shared_ptr<Expressions::Expression> var =
            std::make_shared<Expressions::VariableReference>(
                consumeOrError(TokenType::Identifier).value);
        expr = make_unique<Expressions::Binary>(
            std::move(expr), BinaryOperatorType::MemberAccess, std::move(var));
      }
    }
    if (consume(TokenType::Increment))
      expr = make_unique<Expressions::Unary>(std::move(expr),
                                             UnaryOperatorType::PostIncrement);
    if (consume(TokenType::Decrement))
      expr = make_unique<Expressions::Unary>(std::move(expr),
                                             UnaryOperatorType::PostDecrement);
    return expr;
  }
  if (match(TokenType::StringLiteral)) {
    return make_unique<Expressions::StringLiteral>(
        consumeOrError(TokenType::StringLiteral).value);
  }
  if (match(TokenType::FalseKeyword) || match(TokenType::TrueKeyword)) {
    return make_unique<Expressions::BooleanLiteral>(
        // if the Token is "true", consume(TrueKeyword) returns true, if the
        // Token is "false", it returns false.
        consume(TokenType::TrueKeyword));
  }
  if (consume(TokenType::LParen)) {
    shared_ptr<Expressions::Expression> expr =
        make_shared<Expressions::Parenthed>(mExpression());
    consumeOrError(TokenType::RParen);
    while (consume(TokenType::Period)) {
      if (next().type == TokenType::LParen) {
        expr = make_unique<Expressions::Binary>(
            std::move(expr), BinaryOperatorType::MemberAccess, mFunctionCall());
      } else {
        expr = make_unique<Expressions::Binary>(
            std::move(expr), BinaryOperatorType::MemberAccess,
            make_unique<Expressions::VariableReference>(
                consumeOrError(TokenType::Identifier).value));
      }
    }
    return expr;
  }
  if (consume(TokenType::Hyphen)) {
    return make_unique<Expressions::Unary>(mPrimaryExpression(),
                                           UnaryOperatorType::Minus);
  }
  if (consume(TokenType::Decrement)) {
    return make_unique<Expressions::Unary>(mPrimaryExpression(),
                                           UnaryOperatorType::PreDecrement);
  }
  if (consume(TokenType::Plus)) {
    return make_unique<Expressions::Unary>(mPrimaryExpression(),
                                           UnaryOperatorType::Plus);
  }
  if (consume(TokenType::Increment)) {
    return make_unique<Expressions::Unary>(mPrimaryExpression(),
                                           UnaryOperatorType::PreIncrement);
  }
  mParserError(NonTerminalType::Expression, mCurrentToken());
}

shared_ptr<Expressions::Expression> Parser::mSecondaryExpression() {
  shared_ptr<Expressions::Expression> expr = mPrimaryExpression();
  do {
    if (match(TokenType::Asterisk)) {
      consume();
      expr = make_unique<Expressions::Binary>(
          std::move(expr), BinaryOperatorType::Multiplication,
          mPrimaryExpression());
    }

  } while (getBinaryOperatorType(mCurrentToken()) ==
           BinaryOperatorType::Multiplication);
  return expr;
}

shared_ptr<Expressions::FunctionCall> Parser::mFunctionCall() {
  vector<std::shared_ptr<Expressions::Expression>> params;
  string name = consumeOrError(TokenType::Identifier).value;
  consumeOrError(TokenType::LParen);
  if (consume(TokenType::RParen)) {
    return make_shared<Expressions::FunctionCall>(name, std::move(params));
  }
  do {
    params.push_back(mExpression());
  } while (consume(TokenType::Comma));
  consumeOrError(TokenType::RParen);
  return std::make_unique<Expressions::FunctionCall>(name, std::move(params));
}
};  // namespace Ela
