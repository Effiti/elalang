#include <llvm/IR/Instructions.h>
#include <llvm/IR/PassManager.h>
#include "llvm-includes.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include "../parser/Expression.h"
#include "../parser/Statement.h"
#include "../analysis/Type.h"

namespace Ela::Emitter {
class Emitter {
  const std::unique_ptr<llvm::LLVMContext> llvmContext;
  llvm::IRBuilder<>* irBuilder = new llvm::IRBuilder<>(*llvmContext);
  llvm::Module* irModule;
  std::unique_ptr<llvm::FunctionPassManager> fpm = std::make_unique<llvm::FunctionPassManager>();
  std::unique_ptr<llvm::FunctionAnalysisManager> fam = std::make_unique<llvm::FunctionAnalysisManager>();
  std::map<std::string, llvm::AllocaInst*> namedValues;
  Statements::Program program;

 public:
  std::nullptr_t emitterError(std::string str) {
    throw std::runtime_error("emitter error: " + str);
    return nullptr;
  }
  Emitter() : 
     llvmContext(std::make_unique<llvm::LLVMContext>()),
     irModule(new llvm::Module("my cool JIT", *llvmContext)), irBuilder(new llvm::IRBuilder<>(*llvmContext)){
    }; 
  llvm::Value *integerLiteralValue(int v);
  llvm::Value *characterLiteralValue(char v);
  llvm::Value *varRef(const std::string& str);
  llvm::Value *binary(const Expressions::Binary& bin);
  llvm::Value *functionCall(Expressions::FunctionCall& call);
  llvm::Value *unary(Expressions::Unary &);
  llvm::Value *boolean(bool b);
  llvm::Value *assign(const Expressions::VariableAssign &e);

  llvm::Value *ifStmt(const Statements::IfStatement &s);
  llvm::Value *block(const Statements::BlockStatement &s);
  llvm::Value *ret(const Statements::ReturnStatement &s);
  llvm::Value *forLoop(const Statements::ForStatement &s);
  llvm::Value *varDef(const Statements::VariableDefinitionStatement &s);

  llvm::Function* function(const Statements::FunctionDefinition& def);


  llvm::AllocaInst* createEntryBlockAlloca(llvm::Function *function, const string &name, llvm::Type* type);
  llvm::Type *simpleType(FundamentalType& type);
  llvm::Type *pointerTypeTo(Analysis::Type& type);

  void codegen(const Statements::Program& program);
  void compile(const Statements::Program& program, const std::string &outputFile);
};
}  // namespace Ela::Emitter
