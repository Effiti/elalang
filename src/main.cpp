#include "Ela.hpp"
#include "Lexer.h"
#include "Parser.h"
#include "cmdlib/common.hpp"
#include <string_view>
#include <numeric>

using namespace std::string_view_literals;

int main() {
    std::ifstream ifs("main.ela");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    Lexer l{
            std::string_view(content),
    };
    auto tokens = l.parseSource();
    for (auto tok: tokens) {
        std::cout << CmdUtil::colors["green"] << humanReadableTokenType(tok.type)
                  << CmdUtil::colors["end"] << " at " << tok.line << ":" << tok.col
                  << " : ";
        std::cout << tok.value << "\n";
    }
    ParserOpts opts{10};
    Parser p{tokens, opts};
    //p.parse();
    std::optional<Statements::Programm> program = p.parse();
    if(program == std::nullopt) {
        return EXIT_FAILURE;
    }
    for(const Statements::ImportStatement &imp : program->importStatements.subNodes) {
        std::cout<< "importStatement: "<<  imp.mod << std::endl;
    }
    for(auto &def : program->functionDefinitions.subNodes) {
        std::string params;
        for_each(begin(def.parameters.subNodes), end(def.parameters.subNodes), [&](Statements::Parameter p) {
            params += " " + p.parameterType.to_string() + ":" + std::string(p.parameterName);

        });

        std::cout << "functionDefinition: " << def.functionName << "(" <<params << ") -> " << def.returnType->to_string() << std::endl;
    }


    return EXIT_SUCCESS;
}
