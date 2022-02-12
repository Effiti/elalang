#include "Ela.hpp"
#include "Lexer.hpp"
using namespace std::string_view_literals;

int main() {
  Lexer l{"var a; print {\"Hallo Wel\"}"sv, 4};
  for (auto tok : l.parseSource()) {
    std::cout << humanReadableTokenType(tok.type) << " : "
              << CmdUtil::colors["bg:black"] << tok.lexeme
              << CmdUtil::colors["end"] << "\n";
  }
  return 0;
}
