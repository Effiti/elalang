#pragma once
#include "../Ela.hpp"
#include "../parser/Statement.h"
#include "./SymbolTable.h"

namespace Ela::Analysis {

class ExpressionVisitor {};

class StatementVisitor {
private:
  VariableSymbolTable variables;
  ExpressionVisitor expressionVisitor;
public:
  void visitBlock(Statements::BlockStatement s);
  void visitVariableDefinition(Statements::VariableDefinitionStatement s);
};

} // namespace Ela::Analysis
// DIY_SCANNER_VISITOR_H
