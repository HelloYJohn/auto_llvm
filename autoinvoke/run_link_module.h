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
struct PFDSType {
    std::string pfdsType;
    void* pfdsValue;
    PFDSType() {
        pfdsType = "NULL";
        pfdsValue = NULL;
    }
    PFDSType(std::string pfdsT, void* pfdsV) {
        pfdsType = pfdsT;
        pfdsValue = pfdsV;
    }
};

struct FuncType {
    int64_t funcId;
    std::vector<PFDSType>* paraList;
    PFDSType *ret;
    FuncType():funcId(0), paraList(NULL), ret(NULL) {
    }
    FuncType(int64_t fid, std::vector<PFDSType>* pl, PFDSType* r):funcId(fid), paraList(pl), ret(r) {
    }
};


class MiniLlvmEngine {
    private:
        llvm::LLVMContext* m_llvm_context;
        llvm::Module* m_mod;
        llvm::ExecutionEngine* m_ee;
        void* m_so;
    public:
        MiniLlvmEngine() {
            m_llvm_context = new llvm::LLVMContext();
            m_mod = new llvm::Module("MiniLlvm", *m_llvm_context);
            m_ee = NULL;
            m_so = NULL;
        }
        void init();
        void execute(FuncType& funcType);
        ~MiniLlvmEngine() {
            if (m_ee != NULL) {
                delete m_ee;
            }
            if (m_llvm_context != NULL) {
                delete m_llvm_context;
            }
           llvm::llvm_shutdown();
        }
};

