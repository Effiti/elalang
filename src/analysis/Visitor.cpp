#include "Visitor.h"

namespace Ela {
namespace Analysis {
using std::unique_ptr;
void StatementVisitor::visitVariableDefinition(
    const Statements::VariableDefinitionStatement& s) {
  if (variables.hasSymbol(s.name))
    std::cerr << colors["yellow"] << "WARN: overriding local variable"
              << colors["end"] << std::endl;

  auto symbol = VariableDefinitionSymbol{nesting, s.name, s.type, s.value};

  std::cout << "adding variable " << s.toString() << std::endl;
  variables.add(symbol);
}

void StatementVisitor::visitBlock(const Statements::BlockStatement& block) {
  nesting++;
  for (auto const& s : block.subNodes) {
    s.get()->accept(this);
  }
  nesting--;
}
void ProgramVisitor::check() {
  auto x = StatementVisitor();
  for (const auto& function : program.functionDefinitions) {
    x.visitBlock(*std::move(function.statements));
    // x.visitBlock(statement);
    // x.visitBlock(statement);
  }
}
}  // namespace Analysis
void Statements::BlockStatement::accept(StatementVisitor* visitor) {
  visitor->visitBlock(*this);
}
void Statements::VariableDefinitionStatement::accept(
    StatementVisitor* visitor) {
  visitor->visitVariableDefinition(*this);
}

}  // namespace Ela
