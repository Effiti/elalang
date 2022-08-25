#include "../src/Ela.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include <gtest/gtest.h>

using namespace std::string_view_literals;

class LexerTest : public ::testing::Test {
protected:
  void SetUp() override {
    std::cout << "Setting up lexer..\n";
    mLexerResult = mLexer.parseSource();
  }

  Lexer mLexer{
      "import \"io\";\n"
      "//Hallo Kommentar\n"
      "var int a;\n"
      "a = 10; print a;\n"sv,
  };
  std::vector<Token> mLexerResult;
};
TEST_F(LexerTest, RecognizeImport) {
  EXPECT_EQ(mLexerResult.at(0).type, TokenType::ImportKeyword)
      << "import Keyword was not recognized";
}
TEST_F(LexerTest, ParseStringLiteral) {
  EXPECT_EQ(mLexerResult.at(1).type, TokenType::StringLiteral)
      << "String literal was not recognized";
  EXPECT_EQ(mLexerResult.at(1).value, "io")
      << "String literal was not parsed correctly";
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
