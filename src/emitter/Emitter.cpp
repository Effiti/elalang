#include "Emitter.h"
#include <llvm/IR/Module.h>

namespace Ela::Emitter {
  void Emitter::compile(const Statements::Program& program, const std::string &outputFile) {
    using namespace llvm;
    codegen(program);
    
    auto TargetTriple = sys::getDefaultTargetTriple();

    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();


    std::string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
      errs() << Error;
      throw;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, Reloc::PIC_);

    irModule->setDataLayout(TargetMachine->createDataLayout());
    irModule->setTargetTriple(TargetTriple);

    
    std::error_code EC;
    raw_fd_ostream dest(outputFile, EC, sys::fs::OF_None);

    if (EC) {
      errs() << "Could not open file: " << EC.message();
      throw;
    }

    
    legacy::PassManager pass;
    auto FileType = CodeGenFileType::ObjectFile;

    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
      errs() << "TargetMachine can't emit a file of this type";
      throw;
    }

    pass.run(*irModule);
    dest.flush();
   }
}
