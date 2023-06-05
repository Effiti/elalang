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

class TypeEntry {
 public:
  const std::string typeStr;
  const std::shared_ptr<TypeExpressions::TypeExpression> type;
  TypeEntry(string str, std::shared_ptr<TypeExpressions::TypeExpression> tp)
      : typeStr{str}, type{tp} {};
};

class TypeTable {
  std::vector<TypeEntry> types;
  void addBaseType(TypeExpressions::BaseType type);

 public:
  std::optional<TypeEntry> get(std::string name);
  int add(const TypeEntry& type);
  bool hasType(std::string str);
  TypeEntry getType(int id);
  int getType(std::string str);
  static std::size_t getBaseTypeId(TypeExpressions::BaseType b);
  void print() {
    int i = 0;
    for (const auto& entry : types) {
      std::cout << i << " |\t" << entry.typeStr << std::endl;
      i++;
    }
  };
  TypeTable();
};

class VariableDefinitionSymbol {
 public:
  const unsigned int nesting;
  const std::string name;
  const std::size_t type;
  const std::shared_ptr<Expressions::Expression> initialValue;
  VariableDefinitionSymbol(
      unsigned int nesting_, std::string name_, unsigned int typeId,
      std::shared_ptr<Expressions::Expression> initialValue_)
      : nesting{nesting_},
        name{name_},
        type{typeId},
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
  std::optional<VariableDefinitionSymbol> get(std::string name) const;
  void add(const VariableDefinitionSymbol& symbol);
  bool hasSymbol(std::string name) const;
  void removeAllHigherThan(int nesting);
  void print() {
    int i = 0;
    for (const auto& entry : symbols) {
      std::cout << entry.name << " |\t" << entry.type << std::endl;
      i++;
    }
  };
};
class FunctionDefinitionSymbol {
 public:
  const unsigned int nesting;
  const std::string name;
  const std::size_t type;
  const std::vector<std::size_t> argTypes;
  FunctionDefinitionSymbol(
      unsigned int nesting_, std::string name_, std::size_t typeId,
      std::vector<std::size_t> argTypes)
      : nesting{nesting_},
        name{name_},
        type{typeId},
        argTypes{argTypes} {};
  FunctionDefinitionSymbol(FunctionDefinitionSymbol& s)
      : nesting{s.nesting},
        name{s.name},
        argTypes{s.argTypes},
        type{std::move(s.type)} {};
  FunctionDefinitionSymbol(const FunctionDefinitionSymbol&) = default;
  FunctionDefinitionSymbol operator=(const FunctionDefinitionSymbol& s) {
    return FunctionDefinitionSymbol(s.nesting, s.name, s.type, s.argTypes);
  }
};
class FunctionSymbolTable {
  std::vector<FunctionDefinitionSymbol> symbols;

 public:
  std::optional<FunctionDefinitionSymbol> get(std::string name) const;
  void add(const FunctionDefinitionSymbol& symbol);
  bool hasSymbol(std::string name) const;
  void removeAllHigherThan(int nesting);
  void print() {
    int i = 0;
    for (const auto& entry : symbols) {
      std::cout << entry.name << " |\t"
                << "(...ars)" << entry.type << std::endl;
      std::cout << "args=";
      for (const auto& arg : entry.argTypes) {
        std::cout << arg << ",";
      }
      std::cout << std::endl;
      i++;
    }
  };
};
};  // namespace Ela::Analysis
