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
#include <dlfcn.h>
#include <stdio.h>

using namespace llvm;

Module* makeLLVMModule(Module* mod);
typedef int (*fnGetCalcResult)(int a, int b, void* dl_handle);
typedef bool (*invoke)(const char* str, void* dl_handle);


void* load_shared_lib() {
    void *dl_handle;
    char *error;

    /* Open the shared object */
    dl_handle = dlopen("./libdeep.so", RTLD_LAZY );
    if (!dl_handle) {
        printf( "!!! %s\n", dlerror() );
        return NULL;
    } 

}

int load_bc(const char* str)
{
    void * dl_handle = load_shared_lib();
    llvm_start_multithreaded();
    InitializeNativeTarget();

    Module* main_mod = new Module("invoke.bc", getGlobalContext());

    LLVMContext context;
    SMDiagnostic error;
    Module* mod = ParseIRFile("load_cpp_bc.bc", error, context);

    std::string link_error_code = "";	
    mod = makeLLVMModule(mod);
    Linker::LinkModules(main_mod , mod , 1 , &link_error_code) ;

    std::string error_code = "";
    EngineBuilder eb = EngineBuilder(main_mod);
    //EngineBuilder eb = EngineBuilder(mod);
    eb.setEngineKind(EngineKind::JIT);
    eb.setErrorStr(&error_code);
    ExecutionEngine* EE = eb.create();

    Function* func = NULL ;
    func =  main_mod->getFunction("invoke");
    if( func == NULL)
    {
        std::cout<< "get function error.." << std::endl;
        return 0;
    }

    void* jitAddr = EE->getPointerToFunction(func);
    invoke jitFunc = reinterpret_cast<invoke>(jitAddr);
    bool result = jitFunc("invoke", dl_handle);
    std::cout << "result is :" << result << std::endl;

    EE->freeMachineCodeForFunction(func);
    delete EE;
    llvm_shutdown();
    return 0;
}


Module* makeLLVMModule(Module* mod) {
    // Module Construction
    mod->setDataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128");
    mod->setTargetTriple("x86_64-unknown-linux-gnu");

    // Type Definitions
    std::vector<Type*>FuncTy_0_args;
    PointerType* PointerTy_1 = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

    FuncTy_0_args.push_back(PointerTy_1);
    FuncTy_0_args.push_back(PointerTy_1);
    FunctionType* FuncTy_0 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 1),
            /*Params=*/FuncTy_0_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_2 = PointerType::get(PointerTy_1, 0);

    // std::vector<Type*>FuncTy_4_args;
    // FuncTy_4_args.push_back(IntegerType::get(mod->getContext(), 32));
    // FuncTy_4_args.push_back(IntegerType::get(mod->getContext(), 32));
    // FuncTy_4_args.push_back(PointerTy_1);
    // FunctionType* FuncTy_4 = FunctionType::get(
    //         /*Result=*/IntegerType::get(mod->getContext(), 32),
    //         /*Params=*/FuncTy_4_args,
    //         /*isVarArg=*/false);

    // PointerType* PointerTy_3 = PointerType::get(FuncTy_4, 0);


    // Function Declarations

    Function* func_invoke = mod->getFunction("invoke");
    if (!func_invoke) {
        func_invoke = Function::Create(
                /*Type=*/FuncTy_0,
                /*Linkage=*/GlobalValue::ExternalLinkage,
                /*Name=*/"invoke", mod); 
        func_invoke->setCallingConv(CallingConv::C);
    }
    AttributeSet func_invoke_PAL;
    {
        SmallVector<AttributeSet, 4> Attrs;
        AttributeSet PAS;
        {
            AttrBuilder B;
            B.addAttribute(Attribute::ZExt);
            PAS = AttributeSet::get(mod->getContext(), 0U, B);
        }

        Attrs.push_back(PAS);
        {
            AttrBuilder B;
            B.addAttribute(Attribute::UWTable);
            PAS = AttributeSet::get(mod->getContext(), ~0U, B);
        }

        Attrs.push_back(PAS);
        func_invoke_PAL = AttributeSet::get(mod->getContext(), Attrs);

    }
    func_invoke->setAttributes(func_invoke_PAL);

    // if (!func_add) {
    //     std::cout << "add is empty!" << std::endl;
    //     func_add = Function::Create(
    //             /*Type=*/FuncTy_4,
    //             /*Linkage=*/GlobalValue::ExternalLinkage,
    //             /*Name=*/"add", mod); // (external, no body)
    //     func_add->setCallingConv(CallingConv::C);
    // }
    // AttributeSet func_add_PAL;
    // {
    //     SmallVector<AttributeSet, 4> Attrs;
    //     AttributeSet PAS;
    //     {
    //         AttrBuilder B;
    //         PAS = AttributeSet::get(mod->getContext(), ~0U, B);
    //     }

    //     Attrs.push_back(PAS);
    //     func_add_PAL = AttributeSet::get(mod->getContext(), Attrs);

    // }
    // func_add->setAttributes(func_add_PAL);
    // func_add->addAttribute(llvm::AttributeSet::FunctionIndex, llvm::Attribute::AlwaysInline);

    // Global Variable Declarations


    // Constant Definitions
    ConstantInt* const_int32_5 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt* const_int32_6 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
    ConstantInt* const_int1_7 = ConstantInt::get(mod->getContext(), APInt(1, StringRef("-1"), 10));

    // Global Variable Definitions

    // Function Definitions

    // Function: invoke (func_invoke)
    {
        Function::arg_iterator args = func_invoke->arg_begin();
        Value* ptr_funName = args++;
        ptr_funName->setName("funName");
        Value* ptr_handle = args++;
        ptr_handle->setName("handle");

        BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",func_invoke,0);

        // Block entry (label_entry)
        AllocaInst* ptr_funName_addr = new AllocaInst(PointerTy_1, "funName.addr", label_entry);
        ptr_funName_addr->setAlignment(8);
        AllocaInst* ptr_handle_addr = new AllocaInst(PointerTy_1, "handle.addr", label_entry);
        ptr_handle_addr->setAlignment(8);
        StoreInst* void_8 = new StoreInst(ptr_funName, ptr_funName_addr, false, label_entry);
        void_8->setAlignment(8);
        StoreInst* void_9 = new StoreInst(ptr_handle, ptr_handle_addr, false, label_entry);
        void_9->setAlignment(8);
        LoadInst* ptr_10 = new LoadInst(ptr_handle_addr, "", false, label_entry);
        ptr_10->setAlignment(8);
        Function* func_add = mod->getFunction("add");
        std::vector<Value*> int32_call_params;
        int32_call_params.push_back(const_int32_5);
        int32_call_params.push_back(const_int32_6);
        int32_call_params.push_back(ptr_10);
        CallInst* int32_call = CallInst::Create(func_add, int32_call_params, "call", label_entry);
        int32_call->setCallingConv(CallingConv::C);
        int32_call->setTailCall(false);
        AttributeSet int32_call_PAL;
        int32_call->setAttributes(int32_call_PAL);

        ReturnInst::Create(mod->getContext(), const_int1_7, label_entry);

    }

    return mod;
}
