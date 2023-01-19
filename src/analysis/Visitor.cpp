#include "Visitor.h"

namespace Ela {
namespace Analysis {
using std::unique_ptr;

void StatementVisitor::visitVariableDefinition(
    Statements::VariableDefinitionStatement s) {


}

void StatementVisitor::visitBlock(Statements::BlockStatement block) {
  for (unique_ptr<Statements::Statement> const &s : block.subNodes) {
    s->accept(this);
  }
}
} // namespace Analysis
} // namespace Ela
