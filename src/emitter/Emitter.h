#include "../parser/Expression.h"

namespace Ela::Emitter {
class Emitter {
  const std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::Module> TheModule;
  std::map<std::string, llvm::Value *> NamedValues;

 public:
  llvm::Value *LogErrorV(std::string str) {
    std::cerr << "emitter error: " << str << std::endl;
    return nullptr;
  }
  Emitter() : TheContext() {}
  llvm::Value *integerLiteralValue(int v);
};
}  // namespace Ela::Emitter
