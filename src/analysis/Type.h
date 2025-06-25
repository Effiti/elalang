#pragma once
#include <memory>
#include <variant>
#include <string>

#include "../parser/TypeExpression.h"
namespace Ela::Analysis {
struct Type;

struct ArrayType {
  std::shared_ptr<Type> base;
  ArrayType(std::shared_ptr<Type> base) : base(std::move(base)) {}
  bool operator==(const ArrayType& other) const;
};

struct PointerType {
  std::shared_ptr<Type> base;
  PointerType(std::shared_ptr<Type> base) : base(std::move(base)) {}
  bool operator==(const PointerType& other) const;
};

// struct GenericType {
//   std::string className;
//   std::vector<Type> typeArgs;
//   GenericType(std::string className, std::vector<Type> typeArgs)
//       : className(className), typeArgs(typeArgs) {}
// };

// TODO
struct ClassType {
  std::string className;
  ClassType(std::string className) : className(std::move(className)) {}
  bool operator==(const ClassType& other) const {
    return className == other.className;
  };
};

struct TupleType {
  std::vector<std::shared_ptr<Type>> types;
  TupleType(std::vector<std::shared_ptr<Type>> types)
      : types(std::move(types)) {}
  bool operator==(const TupleType& other) const { return types == other.types; };
};

using TypeData = std::variant<ClassType, Ela::FundamentalType, ArrayType,
                              /*GenericType,*/ PointerType, TupleType>;

class Type {
 public:
  TypeData data;

  // Constructors for each variant
  Type(const ClassType& c) : data(c) {}
  Type(const Ela::FundamentalType& f) : data(f) {}
  Type(const ArrayType& a) : data(a) {}
  // Type(const GenericType& g) : data(g) {}
  Type(const PointerType& p) : data(p) {}
  Type(const TupleType& t) : data(t) {}

  // Comparison operator
  bool operator==(const Type& other) const { return data == other.data; }
  bool operator!=(const Type& other) const { return !(*this == other); }

  // Example: get type kind
  template <typename T>
  bool is() const {
    return std::holds_alternative<T>(data);
  }

  template <typename T>
  const T& as() const {
    return std::get<T>(data);
  }

  std::string toString() const;

  llvm::Type* getIRType(Emitter::Emitter &e) const;

  // Optionally: source location, nullability, etc.
  // TODO: Add more helpers as needed
};

};  // namespace Ela::Analysis
