#pragma once
#include "../Ela.hpp"
namespace Ela::Lexer {
    using std::string;
    using namespace std::string_view_literals;
    enum class TokenType {
        None,
        NumberLiteral,
        StringLiteral,

        LCurly, // done
        RCurly,   // done

        ExplicitConstQualifier,

        Plus,
        Hyphen,
        Caret,
        Asterisk,
        DivisionOperator,
        LParen,
        RParen,
        LBracket,
        RBracket,
        GreaterThanOperator,
        LessThanOperator,
        EqualsOperator,
        BangEqualsOperator,
        ExclamationMark,
        EqualOrGreaterThanOperator,
        EqualOrLessThanOperator,
        Increment,
        Decrement,
        HyphenArrow,
        AssignmentOperator,
        Period,
        Comma,
        Colon,


        Semicolon,
        VariableKeyword,
        ImportKeyword,
        FunctionKeyword,
        IfKeyword,
        ElseKeyword,
        TrueKeyword,
        FalseKeyword,
        ReturnKeyword,

        Identifier,

        Unimplemented,

        EndOfFile

    };

    std::string humanReadableTokenType(TokenType type);

    class Token {
    public:
        Token(std::size_t l, std::size_t c, TokenType t, std::string_view v)
                : line{l}, col{c}, type{t}, value{v}, file{"main.ela"sv} {};
        std::size_t line;
        std::size_t col;
        TokenType type;
        // TODO: change value's type to std::string_view, imlement "file" field in
        // other files
        std::string value;
        std::string_view file;
    };

#define NULLTOK Token{0,0,TokenType::None, ""sv}
};