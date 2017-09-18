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
#include <map>
#include <utility>
#define __FUNC_ID(x, y) ((((x) & 0xFFFFFFFFFFFFFFFF) << 32) | ((y) & 0xFFFFFFFFFFFFFFFF))
enum LLVM_TYPE {
    LLVM_TYPE_BEGIN = -1,
    LLVM_CHAR,          /* char */
    LLVM_BOOL,          /* bool */
    LLVM_INT,           /* int */
    LLVM_INT64,         /* int64 */
    LLVM_DOUBLE,        /* double */
    LLVM_VOID,          /* void */
    LLVM_PCHAR,         /* char* */
    LLVM_PBOOL,         /* bool* */
    LLVM_PINT,          /* int* */
    LLVM_PINT64,        /* int64* */
    LLVM_PDOUBLE,       /* double* */
    LLVM_PVOID,         /* void* */
    LLVM_CHAR_REF,      /* char& */
    LLVM_BOOL_REF,      /* bool& */
    LLVM_INT_REF,       /* int& */
    LLVM_INT64_REF,     /* int64& */
    LLVM_DOUBLE_REF,    /* double& */
    LLVM_STRING_REF,    /* string& */

    LLVM_TYPE_END
};
typedef struct func_info_ {
    std::vector<LLVM_TYPE> func_type_vec; /*first element is function return type*/
    std::string func_name;
    public:
    func_info_() {func_name = ""; func_type_vec.clear();}
}FUNC_INFO;
std::map<std::string, LLVM_TYPE>& get_trans_table() {
    static std::map<std::string, LLVM_TYPE> info;
    return info;
}
LLVM_TYPE transfer2type(std::string& type);
void split(std::string &str, std::vector<LLVM_TYPE>& result);
void dealFuncType(std::string& str, std::vector<LLVM_TYPE>& func_type);
