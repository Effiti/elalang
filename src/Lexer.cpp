#include "Lexer.h"
#include "Ela.hpp"
#include "Token.h"
#include "cmdlib/logging.hpp"
#include <cctype>
#include <string>

// NOTE: CLEAN
void Lexer::logLexerError(std::string errorMessage) {
    mLogger.log(logger::messageType::FATAL_ERROR, errorMessage, mCurrentLine,
                __FILE__);
}

// NOTE: CLEAN
[[nodiscard]] bool Lexer::consume(char expectedCharacter) {
    if (check(expectedCharacter)) {
        consume();
        return true;
    }
    return false;
}

// NOTE: CLEAN
void Lexer::mAddToken(Token token) { mTokens.push_back(token); }

// NOTE: CLEAN
constexpr void Lexer::mNextLine() {
    mCurrentLine++;
    // we have to increment the CurrentPos, as '\n' is counted as a character.
    mCurrentPos++;
    mCurrentCol = 0;
}

// NOTE: CLEAN
constexpr void Lexer::mNextChar() {
    mCurrentCol++;
    mCurrentPos++;
}

// NOTE: CLEAN
void Lexer::consume() {
    if (check('\n')) {
        mNextLine();
    } else
        mNextChar();
}

// NOTE: CLEAN
constexpr char Lexer::peek() const { return mStrSource.at(mCurrentPos); }

// NOTE: CLEAN
constexpr std::string_view Lexer::peek(std::size_t len) {
    return mStrSource.substr(mCurrentPos, len);
}

// NOTE: CLEAN
constexpr char Lexer::peekNext() const {
    return mStrSource.at(mCurrentPos + 1);
}

// NOTE: CLEAN
constexpr bool Lexer::isOverEnd() const {
    return (mCurrentPos > mStrSource.length() - 1);
}

constexpr bool Lexer::isAtEnd() const {
    return (mCurrentPos == mStrSource.length() - 1);
}

constexpr bool Lexer::isUnderEnd() const {
    return (mCurrentPos <= mStrSource.length() - 1);
}

constexpr bool Lexer::isSaveToPeek() const { return isUnderEnd(); }

// NOTE: CLEAN
constexpr bool Lexer::check(char expected) const { return peek() == expected; }

// NOTE: CLEAN
constexpr bool Lexer::mIsWordChar(char input) const {
    return (mIsWordBeginChar(input) || std::isdigit(input));
}

// NOTE: CLEAN
constexpr bool Lexer::mIsWordBeginChar(char input) const {
    return !(std::isspace(input)) && (std::isalpha(input) || input == '_');
}

// TODO: Implement
std::string Lexer::mUnescapeStringLiteral(std::string escapedString) {
    return escapedString;
}

Token Lexer::mEqualsChar() {
    std::size_t line = mCurrentLine, col = mCurrentLine;
    if (mStrSource.length() > mCurrentPos - 1 &&
        mStrSource.at(mCurrentPos + 1) == '=') {
        consume();
        consume();
        return Token{line, col, TokenType::EqualsOperator, "=="};

    } else {
        consume();
        return Token{line, col, TokenType::AssignmentOperator, "="};
    }
}

// NOTE: CLEAN
std::string Lexer::mWordToken() {
    std::string lexeme;
    while (mIsWordChar(peek())) {
        lexeme += peek();
        consume();
    }
    return lexeme;
}

Token Lexer::mMakeWordToken(std::size_t line, std::size_t col,
                            const std::string &lexeme) {
    if (lexeme == "var") {
        return Token{line, col, TokenType::VariableKeyword, ""};
    }
    if (lexeme == "import") {
        Token t = Token{line, col, TokenType::ImportKeyword, ""};
        return t;
    }
    if (lexeme == "func") {
        return Token{line, col, TokenType::FunctionKeyword, ""};
    }

    return Token{line, col, TokenType::Identifier, lexeme};
}

std::string Lexer::mStringLiteralCharacter() {
    std::string value;
    value += peek();
    if (consume('\\')) {
        value += peek();
        consume();
    } else {
        consume();
    }
    return value;
}

Token Lexer::mNumberLiteralToken() {
    std::string value;
    std::size_t line = mCurrentLine, col = mCurrentCol;
    while (isSaveToPeek() && std::isdigit(peek())) {
        value += peek();
        consume();
    }
    return Token{line, col, TokenType::NumberLiteral, value};
}

Token Lexer::mStringLiteralToken() {
    // consume first '"'
    consume();
    std::string value, buf;
    std::size_t line = mCurrentLine, col = mCurrentCol;
    bool stringEnded{false};
    while (!stringEnded && isUnderEnd()) {
        buf = mStringLiteralCharacter();
        if (buf == "\"") {
            stringEnded = true;
            break;
        }
        value += buf;
    }
    if (isOverEnd() && !stringEnded) {
        // std::cout << std::string(1, peek());
        logLexerError("unexpected EOF in parsing string literal token!");
    }
    return mMakeStringLiteralToken(line, col, value);
}

Token Lexer::mMakeStringLiteralToken(std::size_t line, std::size_t col,
                                     const std::string &stringContent) {
    return Token{line, col, TokenType::StringLiteral, stringContent};
}

std::vector<Token> Lexer::parseSource() {
    char currentChar = peek();
    while (isUnderEnd()) {
        currentChar = peek();
        std::size_t line = mCurrentLine, col = mCurrentCol;
        if (consume(' ') || consume('\t') || consume('\n')) {
            if (isOverEnd())
                break;
            continue;
        }
        if (mIsWordBeginChar(peek())) {
            mAddToken(mMakeWordToken(line, col, mWordToken()));
        } else if (peek() == '/') {
            if (mStrSource.length() > mCurrentPos &&
                mStrSource.at(mCurrentPos + 1) == '/') {
                while (mCurrentPos < mStrSource.length() && !check('\n')) {
                    consume();
                }
            } else {
                mAddToken(Token{line, col, TokenType::DivisionOperator, "/"});
                consume();
            }
        } else if (std::isdigit(peek())) {
            mAddToken(mNumberLiteralToken());
        } else if (peek() == '=') {
            mAddToken(mEqualsChar());
        } else if (peek() == '"') {
            mAddToken(mStringLiteralToken());
        } else if (consume('-')) {
            if (consume('>'))
                mAddToken({line, col, TokenType::HyphenArrow, ""});
            else
                mAddToken({line, col, TokenType::Hyphen, ""});
        } else if (consume('*')) {
            mAddToken({line, col, TokenType::Asterisk, ""});
        } else if (consume('^')) {
            mAddToken({line, col, TokenType::Caret, ""});
        } else if (consume('(')) {
            mAddToken({line, col, TokenType::LParen, ""});
        } else if (consume(')')) {
            mAddToken({line, col, TokenType::RParen, ""});
        } else if (consume('[')) {
            mAddToken({line, col, TokenType::LBracket, ""});
        } else if (consume('+')) {
            mAddToken({line, col, TokenType::Plus, ""});
        } else if (consume('!')) {
            if (consume('='))
                mAddToken({line, col, TokenType::BangEqualsOperator, ""});
            else
                mAddToken({line, col, TokenType::ExclamationMark, ""});
        } else if (consume(']')) {
            mAddToken({line, col, TokenType::RBracket, ""});
        } else if (consume('{')) {
            mAddToken({line, col, TokenType::LCurly, ""});
        } else if (consume('}')) {
            mAddToken({line, col, TokenType::RCurly, ""});
        } else if (consume('>')) {
            mAddToken({line, col, TokenType::GreaterThanOperator, ""});
        } else if (consume('<')) {
            mAddToken({line, col, TokenType::LessThanOperator, ""});
        } else if (consume(';')) {
            mAddToken({line, col, TokenType::Semicolon, ""});
        } else if (consume('.')) {
            mAddToken({line, col, TokenType::Period, ""});
        } else if (consume(',')) {
            mAddToken({line, col, TokenType::Comma, ""});
        } else if (consume(':')) {
            mAddToken({line, col, TokenType::Colon, ""});
        }
    }
    mAddToken(Token{mCurrentLine, mCurrentCol, TokenType::EndOfFile, ""});

    return mTokens;
}
