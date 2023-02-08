#pragma once

#include <map>
#include <memory>

#include "../Ela.hpp"
#include "../parser/Node.h"
#include "../parser/Statement.h"
#include "../parser/TypeExpression.h"
#include "../parser/Expression.h"

namespace Ela::Analysis {
using std::unique_ptr;

class VariableDefinitionSymbol {
 public:
  const uint nesting;
  const std::string name;
  const std::shared_ptr<TypeExpressions::TypeExpression> type;
  const std::shared_ptr<Expressions::Expression> initialValue;
  VariableDefinitionSymbol(
      uint nesting_, std::string name_,
      std::shared_ptr<TypeExpressions::TypeExpression> type_,
      std::shared_ptr<Expressions::Expression> initialValue_)
      : nesting{nesting_},
        name{name_},
        type{type_},
        initialValue{initialValue_} {};
};

class VariableSymbolTable {
  std::vector<VariableDefinitionSymbol> symbols;

 public:
  [[nodiscard]] const bool& has(std::string name);
  std::optional<VariableSymbolTable> get(std::string name);
  void add(const VariableDefinitionSymbol &symbol);
  bool hasSymbol(std::string name);
};

};  // namespace Ela::Analysis
