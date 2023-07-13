#include "Emitter.h"
namespace Ela::Expressions{
  llvm::Value *IntegerLiteral::codegen(Emitter::Emitter& e) {
    return e.integerLiteralValue(value);
  }
  llvm::Value *VariableReference::codegen(Emitter::Emitter& e) {
  }
}
namespace Ela::Emitter {
  llvm::Value * Emitter::integerLiteralValue(int v)  {
    //HACK WTF
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(64, v, true));
  }

}
