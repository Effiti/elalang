#include "SymbolTable.h"
namespace Ela::Analysis {
bool VariableSymbolTable::hasSymbol(std::string name) const {
  auto pos = std::find_if(std::begin(symbols), std::end(symbols),
                          [&name](auto x) { return x.name == name; });
  return pos != std::end(symbols);
}

std::optional<VariableDefinitionSymbol> VariableSymbolTable::get(
    std::string name) const {
    auto pos = std::find_if(std::begin(symbols), std::end(symbols),
                          [&name](auto x) { return x.name == name; });
    if(pos == std::end(symbols)) return std::nullopt;
    return *pos;
}

void VariableSymbolTable::add(const VariableDefinitionSymbol &symbol) {
  symbols.emplace_back(symbol);
}

void VariableSymbolTable::removeAllHigherThan(int nesting) {
  auto newSyms = std::vector<VariableDefinitionSymbol>();
  for (const auto &sym : symbols) {
    if (sym.nesting <= nesting) {
      newSyms.emplace_back(std::move(sym));
    }
  }
  symbols = newSyms;
}

bool TypeTable::hasType(std::string typeStr) {
  auto pos = std::find_if(std::begin(types), std::end(types),
                          [typeStr](auto x) { return x.typeStr == typeStr; });
  return pos != std::end(types);
}

int TypeTable::add(const TypeEntry &type) {
  types.push_back(type);
  return types.size() - 1;
}

TypeEntry TypeTable::getType(int id) { return types[id]; }
int TypeTable::getType(std::string typeStr) {
  for (std::size_t i = 0; i < types.size(); i++) {
    if (types[i].typeStr == typeStr) return i;
  }
  return -1;
}
void TypeTable::addBaseType(TypeExpressions::BaseType type) {
  add(TypeEntry(TypeExpressions::to_string(type),
                std::make_shared<TypeExpressions::SimpleType>(type)));
}
TypeTable::TypeTable() {
  addBaseType(TypeExpressions::Void);
  addBaseType(TypeExpressions::Integer);
  addBaseType(TypeExpressions::Double);
  addBaseType(TypeExpressions::Array);
  addBaseType(TypeExpressions::Boolean);
  addBaseType(TypeExpressions::String);
  addBaseType(TypeExpressions::Char);
  addBaseType(TypeExpressions::Float);
  addBaseType(TypeExpressions::Pointer);
  addBaseType(TypeExpressions::Infer);
  addBaseType(TypeExpressions::Function);

}

std::size_t TypeTable::getBaseTypeId(TypeExpressions::BaseType type) {
  switch (type) {
    case TypeExpressions::Array:
      return 3;
    case TypeExpressions::Integer:
      return 1;
    case TypeExpressions::Boolean:
      return 4;
    case TypeExpressions::String:
      return 5;
    case TypeExpressions::Char:
      return 6;
    case TypeExpressions::Double:
      return 2;
    case TypeExpressions::Float:
      return 7;
    case TypeExpressions::Pointer:
      return 8;
    case TypeExpressions::Infer:
      return 9;
    case TypeExpressions::Function:
      return 10;
    case TypeExpressions::Void:
      return 0;
  }
}

}  // namespace Ela::Analysis
