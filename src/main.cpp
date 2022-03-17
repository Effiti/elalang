#include "Ela.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <string_view>
using namespace std::string_view_literals;

int main() {
  std::ifstream ifs("main.ela");
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  Lexer l{
      std::string_view(content),
  };
  auto tokens = l.parseSource();
  for (auto tok : tokens) {
    std::cout << humanReadableTokenType(tok.type) << " : "
              << CmdUtil::colors["bg:black"] << tok.lexeme
              << CmdUtil::colors["end"] << "\n";
  }
  ParserOpts opts{10};
  Parser p{tokens, opts};
  std::vector<ElaNode> nodes = p.parse();
  for (auto node : nodes) {
  }
  return 0;
}
