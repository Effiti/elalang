#pragma once

#include <map>
#include <memory>

#include "../Ela.hpp"
#include "../parser/Expression.h"
#include "../parser/Node.h"
#include "../parser/Statement.h"
#include "../parser/TypeExpression.h"

namespace Ela::Analysis {
using std::unique_ptr;

class VariableDefinitionSymbol {
 public:
  const unsigned int nesting;
  const std::string name;
  const std::shared_ptr<TypeExpressions::TypeExpression> type;
  const std::shared_ptr<Expressions::Expression> initialValue;
  VariableDefinitionSymbol(
      unsigned int nesting_, std::string name_,
      std::shared_ptr<TypeExpressions::TypeExpression> type_,
      std::shared_ptr<Expressions::Expression> initialValue_)
      : nesting{nesting_},
        name{name_},
        type{type_},
        initialValue{initialValue_} {};
  VariableDefinitionSymbol(VariableDefinitionSymbol& s)
      : nesting{s.nesting},
        name{s.name},
        type{std::move(s.type)},
        initialValue{std::move(s.initialValue)} {};
  VariableDefinitionSymbol(const VariableDefinitionSymbol&) = default;
  VariableDefinitionSymbol operator=(const VariableDefinitionSymbol& s) {
    return VariableDefinitionSymbol(s.nesting, s.name, s.type, s.initialValue);
  }
};

class VariableSymbolTable {
  std::vector<VariableDefinitionSymbol> symbols;

 public:
  [[nodiscard]] const bool& has(std::string name);
  std::optional<VariableSymbolTable> get(std::string name);
  void add(const VariableDefinitionSymbol& symbol);
  bool hasSymbol(std::string name);
  void removeAllLowerThan(int nesting);
};
};  // namespace Ela::Analysis
