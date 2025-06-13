#include <algorithm>
#include <cstdlib>
#include <memory>
#include <numeric>
#include <string_view>
#include <clipp.h>

#include "Ela.hpp"
#include "analysis/Visitor.h"
#include "emitter/Emitter.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

namespace Ela::App {
enum class RunConf { LEXER, PARSER, FULL, ANALYSIS };
struct Settings {
  RunConf runconf;
  std::string filename;
};
std::shared_ptr<Settings> makeSettings(int argc, char * argv[]) {
  using namespace clipp;
  // supplying more thclass n one option does not make sense. We will ignore any
  // option that comes after the first one.
    Settings settings {RunConf::FULL, "main.ela"};
    auto cli = (
        value("input file", settings.filename),
        (
          option("-l").set(settings.runconf, RunConf::LEXER)
          | option("-p").set(settings.runconf, RunConf::PARSER)
          | option("-a").set(settings.runconf, RunConf::ANALYSIS)
          | option("-f").set(settings.runconf, RunConf::FULL)
        )
    );

    if(!parse(argc, argv, cli)) std::cout << make_man_page(cli, argv[0]);
    return std::make_shared<Settings>(settings);
}
}  // namespace Ela::App

using namespace std::string_view_literals;
using namespace Ela;

int main(int argc, char * argv[]) {
  const auto settings = App::makeSettings(argc, argv);
  using std::end, std::begin;
  std::ifstream ifs(settings->filename);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  Ela::Lexing::Lexer l{
      std::string_view(content),
  };
  auto tokens = l.parseSource();
  if (settings->runconf == App::RunConf::LEXER) {
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
  if (settings->runconf == App::RunConf::PARSER) {
    for (const Statements::ImportStatement &imp : program->importStatements) {
      std::cout << "importStatement: " << imp.mod << std::endl;
    }
    for (const auto &def : program->functionDefinitions) {
      std::string params;
      std::for_each(
          begin(def.parameters), end(def.parameters), [&](const auto &p) {
            params +=
                " " + p.parameterName + " : " + p.parameterType->toString();
            params += ",";
          });
      std::cout << "functionDefinition: " << def.functionName << "(" << params
                << ") -> " << def.returnType->toString() << " {" << std::endl;

      std::cout << def.statements->toString();

      std::cout << "}" << std::endl;
    }
    return EXIT_SUCCESS;
  }

  Analysis::ProgramVisitor v = Analysis::ProgramVisitor{*program};
  v.check();
  if (settings->runconf == App::RunConf::ANALYSIS) {
    v.print();
    return EXIT_SUCCESS;
  }
  Emitter::Emitter emitter{};
  emitter.compile(*program, "output.o");

  return EXIT_SUCCESS;
}
