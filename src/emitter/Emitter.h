#include <llvm/IR/IRBuilderFolder.h>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include "../parser/Expression.h"

namespace Ela::Emitter {
class Emitter {
  const std::unique_ptr<llvm::LLVMContext> TheContext;
  const std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
  std::unique_ptr<llvm::Module> TheModule;
  std::map<std::string, llvm::Value *> NamedValues;

 public:
  std::nullptr_t emitterError(std::string str) {
    throw std::runtime_error("emitter error: " + str);
    return nullptr;
  }
  Emitter() : TheContext() {}
  llvm::Value *integerLiteralValue(int v);
  llvm::Value *varRef(const std::string& str);
  llvm::Value *binary(Expressions::Binary& bin);
  llvm::Value *functionCall(Expressions::FunctionCall& call);
  llvm::Value *unary(Expressions::Unary &);


  llvm::Type *simpleType(TypeExpressions::SimpleType& type);
};
}  // namespace Ela::Emitter
