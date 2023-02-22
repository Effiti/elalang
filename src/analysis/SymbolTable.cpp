#include "SymbolTable.h"
namespace Ela::Analysis {
bool VariableSymbolTable::hasSymbol(std::string name) {
  auto pos = std::find_if(std::begin(symbols), std::end(symbols),
                          [&name](auto x) { return x.name == name; });
  return pos != std::end(symbols);
}

void VariableSymbolTable::add(const VariableDefinitionSymbol &symbol) {
  symbols.emplace_back(symbol);
}

void VariableSymbolTable::removeAllLowerThan(int nesting){
  auto newSyms = std::vector<VariableDefinitionSymbol>();
  for(const auto & sym : symbols) {
    if(sym.nesting >= nesting){
      newSyms.emplace_back(std::move(sym));
    }
  }
  symbols = newSyms;
}
}  // namespace Ela::Analysis
