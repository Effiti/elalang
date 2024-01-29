#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>

#include <variant>
#include <vector>

#include "Emitter.h"
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
namespace Ela::Statements {
llvm::Value* BlockStatement::codegen(Emitter::Emitter& e) {
  return e.block(*this);
}
llvm::Value* ReturnStatement::codegen(Emitter::Emitter& e) { return e.ret(*this); }

}  // namespace Ela::Statements
namespace Ela::TypeExpressions {
llvm::Type* SimpleType::getIRType(Emitter::Emitter& e) {
  return e.simpleType(*this);
}
}  //  namespace Ela::TypeExpressions
namespace Ela::Emitter {
llvm::Value* Emitter::block(const Statements::BlockStatement& b) {
  for (const auto& stmt : b.subNodes) {
    llvm::Value* v = stmt->codegen(*this);
    if (stmt->is_return()) {
      return v;
    }
  }
  return nullptr;
}
llvm::Value* Emitter::ret(const Statements::ReturnStatement& s) {
  return s.expression->codegen(*this);
}
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
  // TODO
  switch (expr.op) {
    case UnaryOperatorType::Minus:
      return Builder->CreateUnOp(llvm::Instruction::UnaryOps::FNeg,
                                 expr.expression->codegen(*this));
    default:
      return emitterError("unsupported Unary Operator type");
  }
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
      return Builder->CreateFCmpUEQ(lhs, rhs, "equtmp");
    case BinaryOperatorType::UnEqual:
      return Builder->CreateFCmpUNE(lhs, rhs, "neqtmp");
    case BinaryOperatorType::Greater:
      return Builder->CreateFCmpUGT(lhs, rhs, "gttmp");
    case BinaryOperatorType::Less:
      return Builder->CreateFCmpULT(lhs, rhs, "lttmp");
    case BinaryOperatorType::LessEqual:
      return Builder->CreateFCmpULE(lhs, rhs, "letmp");
    case BinaryOperatorType::GreaterEqual:
      return Builder->CreateFCmpUGE(lhs, rhs, "getmp");

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

llvm::Function* Emitter::function(const Statements::FunctionDefinition& def) {
  std::vector<llvm::Type*> parameterTypes = std::vector<llvm::Type*>();
  for (const auto& param : def.parameters) {
    parameterTypes.push_back(param.parameterType->getIRType(*this));
  }
  auto functionType = llvm::FunctionType::get(def.returnType->getIRType(*this),
                                              parameterTypes, false);
  llvm::Function* function =
      llvm::Function::Create(functionType, llvm::Function::ExternalLinkage,
                             def.functionName, TheModule.get());
  std::size_t i = 0;
  for (auto& Arg : function->args())
    Arg.setName(def.parameters[i++].parameterName);

  llvm::BasicBlock* BB =
      llvm::BasicBlock::Create(*TheContext, "entry", function);
  Builder->SetInsertPoint(BB);

  for (auto& arg : function->args())
    NamedValues[std::string(arg.getName())] = &arg;

  if (llvm::Value* retVal = def.statements->codegen(*this)) {
    Builder->CreateRet(retVal);
    verifyFunction(*function);
    return function;
  }
  function->eraseFromParent();
  emitterError("Failure generating IR code for function " + def.functionName);

  return nullptr;
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
void Emitter::codegen(const Statements::Program& program) {
  for (const auto& def : program.functionDefinitions) {
    this->function(def);
  }
  TheModule->print(llvm::errs(), nullptr);
}
}  // namespace Ela::Emitter
