#include "Emitter.h"

#include <variant>
namespace Ela::Expressions {
llvm::Value* IntegerLiteral::codegen(Emitter::Emitter& e) {
  return e.integerLiteralValue(value);
}
llvm::Value* VariableReference::codegen(Emitter::Emitter& e) {
  return e.varRef(variableName);
}
llvm::Value* Binary::codegen(Emitter::Emitter& e) { return e.binary(*this); }
llvm::Value* FunctionCall::codegen(Emitter::Emitter& e) {
  return e.functionCall(*this);
}
llvm::Value* Unary::codegen(Emitter::Emitter& e) { return e.unary(*this); }
}  // namespace Ela::Expressions
namespace Ela::TypeExpressions {
llvm::Type* SimpleType::getIRType(Emitter::Emitter& e) {
  return e.simpleType(*this);
}
}  //  namespace Ela::TypeExpressions
namespace Ela::Emitter {
llvm::Value* Emitter::integerLiteralValue(int v) {
  // HACK Bit width and unsigned integers -> enum "IntegerType", later
  // autodetection
  return llvm::ConstantInt::get(*TheContext, llvm::APInt(64, v, true));
}
llvm::Value* Emitter::varRef(const std::string& name) {
  llvm::Value* V = NamedValues[name];
  if (!V) emitterError("Unknown variable name");
  return V;
}
llvm::Value* Emitter::unary(Expressions::Unary& expr) {
  //TODO
  return nullptr;
}
llvm::Value* Emitter::binary(Expressions::Binary& expr) {
  llvm::Value *lhs, *rhs;
  lhs = expr.lhs->codegen(*this);
  rhs = expr.rhs->codegen(*this);

  switch (expr.op) {
    case BinaryOperatorType::Plus:
      return Builder->CreateFAdd(lhs, rhs, "addtmp");
    case BinaryOperatorType::Division:
      return Builder->CreateFDiv(lhs, rhs, "divtmp");
    case BinaryOperatorType::Minus:
      return Builder->CreateFSub(lhs, rhs, "subtmp");
    case BinaryOperatorType::Multiplication:
      return Builder->CreateFMul(lhs, rhs, "multmp");
    case BinaryOperatorType::Equal:
      return Builder->CreateFCmpOEQ(lhs, rhs, "equtmp");
    case BinaryOperatorType::RightShift:
    case BinaryOperatorType::LeftShift:
    default:
      return emitterError("unsupported Binary Operator Type");
  }
}
llvm::Value* Emitter::functionCall(Expressions::FunctionCall& call) {
  llvm::Function* callee = TheModule->getFunction(call.functionName);
  if (!callee) return emitterError("unknown function");

  if (callee->arg_size() != call.callParams.size())
    return emitterError("parameter length not matching");
  std::vector<llvm::Value*> args;
  for (std::size_t i = 0; i < call.callParams.size(); ++i) {
    args.push_back(call.callParams[i]->codegen(*this));
  }
  return Builder->CreateCall(callee, args, "calltmp");
}

llvm::Type* Emitter::simpleType(TypeExpressions::SimpleType& type) {
  if (std::holds_alternative<const std::string>(type.type))
    return emitterError("custom types unsupported");
  switch (std::get<TypeExpressions::BaseType>(type.type)) {
    case TypeExpressions::Boolean:
      return llvm::Type::getInt1Ty(*TheContext);
    case TypeExpressions::Integer:
      return llvm::Type::getInt32Ty(*TheContext);
    case TypeExpressions::Null:
    case TypeExpressions::Void:
      return llvm::Type::getVoidTy(*TheContext);
    case TypeExpressions::Double:
      return llvm::Type::getDoubleTy(*TheContext);
    case TypeExpressions::Char:
      return llvm::Type::getInt8Ty(*TheContext);
    case TypeExpressions::Float:
      return llvm::Type::getFloatTy(*TheContext);
    default:
      return emitterError("unimplemented IR Type");
  }
}

}  // namespace Ela::Emitter
