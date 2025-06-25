#include "Type.h"

#include <stdexcept>
#include <variant>

namespace Ela::TypeExpressions {

Analysis::Type TypeExpression::toAnalysisType() {
  throw std::runtime_error(
      "toAnalysisType() not implemented for this TypeExpression");
}

Analysis::Type PointerType::toAnalysisType() {
  return Analysis::Type(Analysis::PointerType(
      std::make_shared<Analysis::Type>(base_type->toAnalysisType())));
}
Analysis::Type SimpleType::toAnalysisType() {
  if (std::holds_alternative<BaseType>(type))
    return Analysis::Type(FundamentalType(std::get<BaseType>(type)));
  if (std::holds_alternative<const std::string>(type))
    return Analysis::Type(
        Analysis::ClassType(std::get<const std::string>(type)));
  throw std::runtime_error("unreachable");
}
Analysis::Type TupleTypeExpression::toAnalysisType() {
  throw std::runtime_error("unimplemented");
}
Analysis::Type TypeTemplateExpression::toAnalysisType() {
  throw std::runtime_error("unimplemented");
}
}  // namespace Ela::TypeExpressions
namespace Ela::Analysis {
bool ArrayType::operator==(const ArrayType& other) const {
  return *base == *other.base;
};
bool PointerType::operator==(const PointerType& other) const {
  return *base == *other.base;
};
std::string Type::toString() const {
  return "to lazy to impl toString for Types";
}
};  // namespace Ela::Analysis
