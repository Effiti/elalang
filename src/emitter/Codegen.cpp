#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/Target/TargetMachine.h>

#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

#include "Emitter.h"
namespace Ela::Expressions {
llvm::Value* IntegerLiteral::codegen(Emitter::Emitter& e) {
  return e.integerLiteralValue(value);
}
llvm::Value* BooleanLiteral::codegen(Emitter::Emitter& e) {
  return e.boolean(value);
}
llvm::Value* VariableReference::codegen(Emitter::Emitter& e) {
  return e.varRef(variableName);
}
llvm::Value* Binary::codegen(Emitter::Emitter& e) {
  auto v = e.binary(*this);
  return v;
}
llvm::Value* FunctionCall::codegen(Emitter::Emitter& e) {
  return e.functionCall(*this);
}
llvm::Value* Unary::codegen(Emitter::Emitter& e) { return e.unary(*this); }
llvm::Value* Parenthed::codegen(Emitter::Emitter& e) {
  return subExpr->codegen(e);
}
llvm::Value* VariableAssign::codegen(Emitter::Emitter& e) {
  return e.assign(*this);
}
}  // namespace Ela::Expressions
namespace Ela::Statements {
llvm::Value* IfStatement::codegen(Emitter::Emitter& e) {
  return e.ifStmt(*this);
}
llvm::Value* BlockStatement::codegen(Emitter::Emitter& e) {
  return e.block(*this);
}
llvm::Value* ReturnStatement::codegen(Emitter::Emitter& e) {
  return e.ret(*this);
}
llvm::Value* ExpressionStatement::codegen(Emitter::Emitter& e) {
  return expression->codegen(e);
}
llvm::Value* ForStatement::codegen(Emitter::Emitter& e) {
  return e.forLoop(*this);
}
llvm::Value* VariableDefinitionStatement::codegen(Emitter::Emitter& e) {
  return e.varDef(*this);
}

}  // namespace Ela::Statements
namespace Ela::TypeExpressions {
llvm::Type* SimpleType::getIRType(Emitter::Emitter& e) {
  return e.simpleType(*this);
}
}  //  namespace Ela::TypeExpressions
namespace Ela::Emitter {
llvm::AllocaInst* Emitter::createEntryBlockAlloca(llvm::Function* TheFunction,
                                                  const std::string& varName,
                                                  llvm::Type* type) {
  llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                         TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(type, nullptr, varName);
}
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
  return llvm::ConstantInt::get(*llvmContext, llvm::APInt(64, v, true));
  // return llvm::ConstantFP::get(*llvmContext, llvm::APFloat((float)v));
}
llvm::Value* Emitter::boolean(bool b) {
  return llvm::ConstantInt::getBool(llvm::Type::getInt1Ty(*llvmContext), b);
}
llvm::Value* Emitter::varRef(const std::string& name) {
  llvm::AllocaInst* ptr = namedValues[name];
  if (!ptr) return emitterError("unknown Variable");
  return irBuilder->CreateLoad(ptr->getAllocatedType(), ptr, name.c_str());
}
llvm::Value* Emitter::unary(Expressions::Unary& expr) {
  // TODO
  switch (expr.op) {
    case UnaryOperatorType::Minus:
      return irBuilder->CreateUnOp(llvm::Instruction::UnaryOps::FNeg,
                                   expr.expression->codegen(*this));
    default:
      return emitterError("unsupported Unary Operator type");
  }
  return nullptr;
}
llvm::Value* Emitter::binary(const Expressions::Binary& expr) {
  llvm::Value *lhs, *rhs;
  lhs = expr.lhs->codegen(*this);
  rhs = expr.rhs->codegen(*this);
  if (!lhs || !rhs) {
    return nullptr;
  }
  // TODO when both sides are constants, we get, SEGFAULT?!??
  //  This is utter nonsense, but oh well, it seems to force us to
  //  const-optimize

  switch (expr.op) {
      // ONLY i64 addition!
    case BinaryOperatorType::Plus: {
      llvm::Value* value = irBuilder->CreateAdd(lhs, rhs, "addtmp");
      return value;
    }
    case BinaryOperatorType::Division:
      return irBuilder->CreateFDiv(lhs, rhs, "divtmp");
    case BinaryOperatorType::Minus:
      return irBuilder->CreateSub(lhs, rhs, "subtmp");
    case BinaryOperatorType::Multiplication:
      return irBuilder->CreateMul(lhs, rhs, "multmp");
    case BinaryOperatorType::Equal:
      return irBuilder->CreateICmpEQ(lhs, rhs, "equtmp");
    case BinaryOperatorType::UnEqual:
      return irBuilder->CreateICmpEQ(lhs, rhs, "neqtmp");
    case BinaryOperatorType::Greater:
      return irBuilder->CreateICmpUGT(lhs, rhs, "gttmp");
    case BinaryOperatorType::Less:
      return irBuilder->CreateICmpULT(lhs, rhs, "lttmp");
    case BinaryOperatorType::LessEqual:
      return irBuilder->CreateICmpULE(lhs, rhs, "letmp");
    case BinaryOperatorType::GreaterEqual:
      return irBuilder->CreateICmpUGE(lhs, rhs, "getmp");

    case BinaryOperatorType::RightShift:
    case BinaryOperatorType::LeftShift:
    default:
      return emitterError("unsupported Binary Operator Type");
  }
}
llvm::Value* Emitter::functionCall(Expressions::FunctionCall& call) {
  llvm::Function* callee = irModule->getFunction(call.functionName);
  if (!callee) return emitterError("unknown function");

  if (callee->arg_size() != call.callParams.size())
    return emitterError("parameter length not matching");
  std::vector<llvm::Value*> args;
  for (std::size_t i = 0; i < call.callParams.size(); ++i) {
    args.push_back(call.callParams[i]->codegen(*this));
  }
  return irBuilder->CreateCall(callee, args, "calltmp");
}
llvm::Value* Emitter::ifStmt(const Statements::IfStatement& stmt) {
  llvm::Value* CondV = stmt.condition->codegen(*this);
  if (!CondV) return nullptr;

  // Convert condition to a bool by comparing non-equal to 0.0.
  CondV = irBuilder->CreateFCmpONE(CondV, CondV, "ifcond");

  llvm::Function* fn = irBuilder->GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(*llvmContext, "then", fn);
  llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(*llvmContext, "ifcont");
  llvm::BasicBlock* ElseBB =
      stmt.elseStatement.has_value()
          ? llvm::BasicBlock::Create(*llvmContext, "else")
          : MergeBB;

  // create br deciding between branches
  irBuilder->CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then label
  irBuilder->SetInsertPoint(ThenBB);

  // emit then block
  llvm::Value* thenBranch = stmt.statement->codegen(*this);
  // if (!thenBranch) return nullptr;

  irBuilder->CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = irBuilder->GetInsertBlock();
  llvm::Value* elseBranch = nullptr;
  if (stmt.elseStatement.has_value()) {
    // Emit else block.
    fn->insert(fn->end(), ElseBB);
    irBuilder->SetInsertPoint(ElseBB);

    elseBranch = stmt.elseStatement.value()->codegen(*this);
    // if (!elseBranch) return nullptr;

    irBuilder->CreateBr(MergeBB);
    // codegen of 'Else' can change the current block, update ElseBB for the
    // PHI.
    ElseBB = irBuilder->GetInsertBlock();
  }
  // Emit merge block.
  fn->insert(fn->end(), MergeBB);
  irBuilder->SetInsertPoint(MergeBB);
  /*llvm::PHINode* PN =
  irBuilder->CreatePHI(llvm::Type::getInt32Ty(*llvmContext),
  stmt.elseStatement.has_value() ? 2 : 1, "iftmp");

  PN->addIncoming(thenBranch, ThenBB);
  if(stmt.elseStatement.has_value())
    PN->addIncoming(elseBranch, ElseBB);
  return PN;
  */
  return nullptr;
}

llvm::Value* Emitter::forLoop(const Statements::ForStatement& s) {
  llvm::Value* init = s.init->codegen(*this);

  llvm::Function* fn = irBuilder->GetInsertBlock()->getParent();
  llvm::BasicBlock* PreheaderBB = irBuilder->GetInsertBlock();
  llvm::BasicBlock* LoopBB = llvm::BasicBlock::Create(*llvmContext, "loop", fn);

  // Insert an explicit fall through from the current block to the LoopBB.
  irBuilder->CreateBr(LoopBB);

  irBuilder->SetInsertPoint(LoopBB);
  s.init->codegen(*this);

  llvm::Value* check = s.check->codegen(*this);
  llvm::Value* body = s.body->codegen(*this);

  // Create the "after loop" block and insert it.
  llvm::BasicBlock* LoopEndBB = irBuilder->GetInsertBlock();
  llvm::BasicBlock* AfterBB =
      llvm::BasicBlock::Create(*llvmContext, "afterloop", fn);

  // Insert the conditional branch into the end of LoopEndBB.
  irBuilder->CreateCondBr(check, LoopBB, AfterBB);

  // Any new code will be inserted in AfterBB.
  irBuilder->SetInsertPoint(AfterBB);
  return nullptr;
}

llvm::Value* Emitter::varDef(
    const Statements::VariableDefinitionStatement& def) {
  llvm::Function* function = irBuilder->GetInsertBlock()->getParent();
  // an ALlocaInst basically returns the llvm::Value of the Pointer returned by
  // allocation
  llvm::AllocaInst* allocaInstance =
      createEntryBlockAlloca(function, def.name, def.type->getIRType(*this));
  llvm::Value* initial = def.value->codegen(*this);

  irBuilder->CreateStore(initial, allocaInstance);
  namedValues[def.name] = allocaInstance;
  return allocaInstance;
}
llvm::Value* Emitter::assign(const Expressions::VariableAssign& e) {
  llvm::Value* value = e.value->codegen(*this);
  llvm::AllocaInst* ptr = namedValues[e.name];
  irBuilder->CreateStore(value, ptr);
  return value;
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
                             def.functionName, irModule);
  std::size_t i = 0;
  for (auto& Arg : function->args())
    Arg.setName(def.parameters[i++].parameterName);
  if (def.isExtern) return function;

  llvm::BasicBlock* BB =
      llvm::BasicBlock::Create(*llvmContext, "entry", function);
  irBuilder->SetInsertPoint(BB);
  i = 0;
  for (auto& arg : function->args()) {
    // Create an alloca for this variable.
    llvm::AllocaInst* allocated = createEntryBlockAlloca(
        function, arg.getName().str(), parameterTypes.at(i));

    // Store the initial value into the alloca.
    irBuilder->CreateStore(&arg, allocated);

    // Add arguments to variable symbol table.
    namedValues[arg.getName().str()] = allocated;
    ++i;
  }

  if (llvm::Value* retVal = def.statements->codegen(*this)) {
    irBuilder->CreateRet(retVal);
    verifyFunction(*function);
    // fpm->run(*function, *fam);

    return function;
  }
  function->eraseFromParent();
  emitterError("Failure generating IR code for function " + def.functionName + ". No return statement?");

  return nullptr;
}

llvm::Type* Emitter::simpleType(TypeExpressions::SimpleType& type) {
  if (std::holds_alternative<const std::string>(type.type))
    return emitterError("custom types unsupported");
  switch (std::get<TypeExpressions::BaseType>(type.type)) {
    case TypeExpressions::Boolean:
      return llvm::Type::getInt1Ty(*llvmContext);
    case TypeExpressions::Integer:
      return llvm::Type::getInt64Ty(*llvmContext);
    case TypeExpressions::Null:
    case TypeExpressions::Void:
      return llvm::Type::getVoidTy(*llvmContext);
    case TypeExpressions::Double:
      return llvm::Type::getDoubleTy(*llvmContext);
    case TypeExpressions::Char:
      return llvm::Type::getInt8Ty(*llvmContext);
    case TypeExpressions::Float:
      return llvm::Type::getFloatTy(*llvmContext);
    default:
      return emitterError("unimplemented IR Type: " + type.toString());
  }
}
void Emitter::codegen(const Statements::Program& program) {
  // Create new pass and analysis managers.
  auto lam = std::make_unique<llvm::LoopAnalysisManager>();
  auto cgam = std::make_unique<llvm::CGSCCAnalysisManager>();
  auto mam = std::make_unique<llvm::ModuleAnalysisManager>();
  auto pic = std::make_unique<llvm::PassInstrumentationCallbacks>();
  auto si =
      std::make_unique<llvm::StandardInstrumentations>(*llvmContext,
                                                       /*DebugLogging*/ true);
  si->registerCallbacks(*pic, mam.get());
  // Add transform passes.
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  fpm->addPass(llvm::InstCombinePass());
  // Reassociate expressions.
  fpm->addPass(llvm::ReassociatePass());
  // Eliminate Common SubExpressions.
  fpm->addPass(llvm::GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  fpm->addPass(llvm::SimplifyCFGPass());

  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(*mam);
  PB.registerFunctionAnalyses(*fam);
  PB.crossRegisterProxies(*lam, *fam, *cgam, *mam);

  for (const auto& def : program.functionDefinitions) {
    this->function(def);
  }
  irModule->print(llvm::errs(), nullptr);
}
}  // namespace Ela::Emitter
