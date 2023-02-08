#include "Ela.hpp"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "analysis/Visitor.h"
#include <algorithm>
#include <numeric>
#include <string_view>

using namespace std::string_view_literals;
using namespace Ela;
int main() {
  using std::end, std::begin;
  std::ifstream ifs("main.ela");
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  Ela::Lexer::Lexer l{
      std::string_view(content),
  };
  auto tokens = l.parseSource();
  for (auto tok : tokens) {
    std::cout << Ela::colors["green"] << humanReadableTokenType(tok.type)
              << Ela::colors["end"] << " at " << tok.line << ":" << tok.col
              << " : ";
    std::cout << tok.value << "\n";
  }
  ParserOpts opts{10};
  Parser p{tokens, opts};
  // p.parse();
  const auto program = p.parse();
  if (!program) {
    return EXIT_FAILURE;
  }
  for (const Statements::ImportStatement &imp : program->importStatements) {
    std::cout << "importStatement: " << imp.mod << std::endl;
  }
  for (const auto &def : program->functionDefinitions) {
    std::string params;
    std::for_each(
        begin(def.parameters), end(def.parameters), [&](const auto &p) {
          params += " " + p.parameterName + " : " + p.parameterType->toString();
          params += ",";
        });
    std::cout << "functionDefinition: " << def.functionName << "(" << params
              << ") -> " << def.returnType->toString() << " {" << std::endl;

    std::cout << def.statements->toString();

    std::cout << "}" << std::endl;
  }
  Analysis::ProgramVisitor v = Analysis::ProgramVisitor{*program};
  v.check();

  return EXIT_SUCCESS;
}
