#include <llvm/IR/IRBuilderFolder.h>
#include <llvm/IR/LLVMContext.h>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include "../parser/Expression.h"
#include "../parser/Statement.h"

namespace Ela::Emitter {
class Emitter {
  const std::unique_ptr<llvm::LLVMContext> llvmContext;
  const std::unique_ptr<llvm::IRBuilder<>> irBuilder = std::make_unique<llvm::IRBuilder<>>(*llvmContext);
  std::unique_ptr<llvm::Module> irModule;
  std::map<std::string, llvm::Value *> namedValues;

 public:
  std::nullptr_t emitterError(std::string str) {
    throw std::runtime_error("emitter error: " + str);
    return nullptr;
  }
  Emitter() : 
     llvmContext(std::make_unique<llvm::LLVMContext>()),
     irModule(std::make_unique<llvm::Module>("my cool JIT", *llvmContext)), irBuilder(std::make_unique<llvm::IRBuilder<>>(*llvmContext)){
      
    }; 
  llvm::Value *integerLiteralValue(int v);
  llvm::Value *varRef(const std::string& str);
  llvm::Value *binary(const Expressions::Binary& bin);
  llvm::Value *functionCall(Expressions::FunctionCall& call);
  llvm::Value *unary(Expressions::Unary &);
  llvm::Value *boolean(bool b);

  llvm::Value *block(const Statements::BlockStatement &s);
  llvm::Value *ret(const Statements::ReturnStatement &s);

  llvm::Function* function(const Statements::FunctionDefinition& def);

  llvm::Type *simpleType(TypeExpressions::SimpleType& type);
  void codegen(const Statements::Program& program);
};
}  // namespace Ela::Emitter
