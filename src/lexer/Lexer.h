#pragma once

#include "../Ela.hpp"
#include "Token.h"
namespace Ela::Lexing {
    class Lexer {
    public:
        explicit Lexer(std::string_view source) : mStrSource{source} {}

        [[nodiscard]] std::vector<Token> parseSource();

        void logLexerError(std::string s);

    private:
        void consume();

        [[nodiscard]] bool consume(char c);

        [[nodiscard]] constexpr std::string_view peek(std::size_t len);

        [[nodiscard]] constexpr char peek() const;

        [[nodiscard]] constexpr char peekNext() const;

        [[nodiscard]] constexpr bool isAtEnd() const;

        [[nodiscard]] constexpr bool isOverEnd() const;

        [[nodiscard]] constexpr bool isUnderEnd() const;

        [[nodiscard]] constexpr bool isSaveToPeek() const;

        [[nodiscard]] constexpr bool check(char c) const;

        std::string_view mStrSource;
        std::size_t mCurrentPos{0};
        std::size_t mCurrentLine{1};
        std::size_t mCurrentCol{1};

        std::vector<Token> mTokens;

        void mAddToken(Token token);

        constexpr void mNextLine();

        constexpr void mNextChar();

        [[nodiscard]] constexpr bool mIsWordChar(char input) const;

        [[nodiscard]] constexpr bool mIsWordBeginChar(char input) const;

        Token mEqualsChar();

        std::string mStringLiteralCharacter();

        std::string mWordToken();

        Token mNumberLiteralToken();

        Token mStringLiteralToken();

        static Token mMakeWordToken(std::size_t line, std::size_t col, const std::string &value);

        static Token mMakeStringLiteralToken(std::size_t line, std::size_t col, const std::string &s);
    };
};
