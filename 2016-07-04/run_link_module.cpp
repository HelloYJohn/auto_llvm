#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/Verifier.h>
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
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Support/MemoryBuffer.h"
#include <algorithm>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Transforms/IPO.h"
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <iostream>
#include <string>
#include <llvm/Linker.h>

using namespace llvm;

typedef int (*fnGetCalcResult)(int a, int b);

int main()
{
        llvm_start_multithreaded();
        InitializeNativeTarget();
	
	 Module* main_mod = new Module("main_module" , getGlobalContext());

        LLVMContext context;
        SMDiagnostic error;
	Module* mod = ParseIRFile("load_cpp_bc.bc", error, context);
	
	std::string link_error_code = "";	
	 Linker::LinkModules(main_mod , mod , 1 , &link_error_code) ;
		
	std::string error_code = "";
	EngineBuilder eb = EngineBuilder(main_mod);
	//EngineBuilder eb = EngineBuilder(mod);
        eb.setEngineKind(EngineKind::JIT);
        eb.setErrorStr(&error_code);
        ExecutionEngine* EE = eb.create();
	
	Function* func = NULL ;
    func =  mod->getFunction("add");
	if( func == NULL)
    	{
        	std::cout<< "--Night-- get function error.." << std::endl;
        	return 0;
   	}

	void* jitAddr = EE->getPointerToFunction(func);
        fnGetCalcResult jitFunc = reinterpret_cast<fnGetCalcResult>(jitAddr);	
        
	int result = jitFunc(3,5);
	std::cout << "result is :" << result << std::endl;
	
	EE->freeMachineCodeForFunction(func);
        delete EE;
        llvm_shutdown();
	return 0;
}
