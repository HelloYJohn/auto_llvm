#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/Support/CFG.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Support/MemoryBuffer.h"
#include <algorithm>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/JIT.h>
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Transforms/IPO.h"
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace llvm;

int main(int argc, char**argv) {
    llvm_start_multithreaded();
    InitializeNativeTarget();
    std::string error_code ;
    LLVMContext &context = getGlobalContext();
    SMDiagnostic Err;

    Module *mod = llvm::parseIRFile("load_c_bc.bc",Err,context);
    EngineBuilder eb = EngineBuilder(mod);
    eb.setEngineKind(EngineKind::JIT);
    eb.setErrorStr(&error_code);
    ExecutionEngine* EE = eb.create();

    Function* func = NULL ;
    func =  mod->getFunction("_Z13CalcVaildRowsPPii");
    if( func == NULL)
    {
        std::cout<< "--Night-- get function error.." << std::endl;
        return 0;
    }

    void* jitAddr = EE->getPointerToFunction(func);
    //fnGetCalcResult jitFunc = reinterpret_cast<fnGetCalcResult>(jitAddr);
    //int rows = jitFunc(mdp,5);

    EE->freeMachineCodeForFunction(func);
    delete EE;
    llvm_shutdown();
    return 0;
}
