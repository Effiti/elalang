#include <array>
#include <memory>
#include <string>
#include <vector>

#include "../parser/TypeExpression.h"
namespace Ela::Analysis {
struct Type;

struct ArrayType {
  std::shared_ptr<Type> base;
};

struct PointerType {
  std::shared_ptr<Type> base;
};

struct GenericType {
  std::string className;
  std::vector<Type> typeArgs;
};

// TODO
struct ClassType {
  std::string className;
};

struct TupleType {
  std::vector<Type> types;
};

using TypeData = std::variant<ClassType, Ela::FundamentalType, ArrayType,
                              GenericType, PointerType, TupleType>;

class Type {
  TypeData data;
  bool cast(const Type &other);
  // Optionally: source location, nullability, etc.
};
};  // namespace Ela::Analysis
