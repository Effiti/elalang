#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <string_view>

#include "Ela.hpp"
#include "analysis/Visitor.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

namespace Ela::App {
const static auto optstring = "lpaf";
enum class RunConf { LEXER, PARSER, FULL, ANALYSIS };
const RunConf getConf(int argc, char *const argv[]) {
  // supplying more than one option does not make sense. We will ignore any option that comes after the first one.
  switch (getopt(argc, argv, optstring)) {
    case 'l':
      return RunConf::LEXER;
    case 'p':
      return RunConf::PARSER;
    case 'a':
      return RunConf::ANALYSIS;
    case 'f':
    default:
      return RunConf::FULL;
  }
}
}  // namespace Ela::App

using namespace std::string_view_literals;
using namespace Ela;

int main(int argc, char *const argv[]) {
  const auto conf = App::getConf(argc, argv);
  using std::end, std::begin;
  std::ifstream ifs("main.ela");
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  Ela::Lexer::Lexer l{
      std::string_view(content),
  };
  auto tokens = l.parseSource();
  if (conf == App::RunConf::LEXER) {
    for (auto tok : tokens) {
      std::cout << Ela::colors["green"] << humanReadableTokenType(tok.type)
                << Ela::colors["end"] << " at " << tok.line << ":" << tok.col
                << " : ";
      std::cout << tok.value << "\n";
    }
    return EXIT_SUCCESS;
  }

  ParserOpts opts{10};
  Parser p{tokens, opts};
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
  if(conf == App::RunConf::PARSER)
    return EXIT_SUCCESS;

  Analysis::ProgramVisitor v = Analysis::ProgramVisitor{*program};
  v.check();

  return EXIT_SUCCESS;
}
