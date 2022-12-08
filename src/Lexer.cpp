#include "Lexer.hpp"
#include "Ela.hpp"
#include "Token.hpp"
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
        return Token{line, col, TokenType::VariableKeyword, lexeme};
    }
    if (lexeme == "import") {
        Token t = Token{line, col, TokenType::ImportKeyword, lexeme};
        return t;
    }
    if (lexeme == "func") {
        return Token{line, col, TokenType::FunctionKeyword, lexeme};
    }
    return Token{line, col, TokenType::Identifier, lexeme};
}

TokenType Lexer::mGetSingleCharTokenType(char input) {
    // TODO put back into parseSource()
    using
    enum TokenType;
    switch (input) {
        case '-':
            return MinusOperator;
        case '*':
            return MultiplicationOperator;
        case '/':
            return DivisionOperator;
        case '^':
            return PowerOperator;
        case '(':
            return ParenthesesOpen;
        case ')':
            return ParenthesesClose;
        case '[':
            return BracketsOpen;
        case ']':
            return BracketsClose;
        case '{':
            return BlockBegin;
        case '}':
            return BlockEnd;
        case '>':
            return GreaterThanOperator;
        case '<':
            return LessThanOperator;
        case ';':
            return Semicolon;
        case '=':
            return EqualsOperator;
        default:
            return None;
    }
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

// WARNING: DEPRECATED
Token Lexer::mMakeSingleCharToken(std::size_t line, std::size_t col, char c) {
    return Token{line, col, mGetSingleCharTokenType(c), std::string(1, c)};
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
            mAddToken(Token{line, col, TokenType::MinusOperator, "-"});
        } else if (consume('*')) {
            mAddToken(Token{line, col, TokenType::MultiplicationOperator, "*"});
        } else if (consume('^')) {
            mAddToken(Token{line, col, TokenType::PowerOperator, "^"});
        } else if (consume('(')) {
            mAddToken(Token{line, col, TokenType::ParenthesesOpen, "("});
        } else if (consume(')')) {
            mAddToken(Token{line, col, TokenType::ParenthesesClose, ")"});
        } else if (consume('[')) {
            mAddToken(Token{line, col, TokenType::BracketsOpen, "]"});
        } else if (consume('+')) {
            mAddToken(Token{line, col, TokenType::PlusOperator, "+"});
        } else if (consume('!')) {
            if (consume('='))
                mAddToken(Token{line, col, TokenType::BangEqualsOperator, "!="});
            else
                mAddToken(Token{line, col, TokenType::BangOperator, "!"});
        } else if (consume(']')) {
            mAddToken(Token{line, col, TokenType::BracketsClose, "]"});
        } else if (consume('{')) {
            mAddToken(Token{line, col, TokenType::BlockBegin, "{"});
        } else if (consume('}')) {
            mAddToken(Token{line, col, TokenType::BlockEnd, "}"});
        } else if (consume('>')) {
            mAddToken(Token{line, col, TokenType::GreaterThanOperator, ">"});
        } else if (consume('<')) {
            mAddToken(Token{line, col, TokenType::LessThanOperator, "<"});
        } else if (consume(';')) {
            mAddToken(Token{line, col, TokenType::Semicolon, ";"});
        }
    }
    mAddToken(Token{mCurrentLine, mCurrentCol, TokenType::EndOfFile, ""});

    return mTokens;
}
