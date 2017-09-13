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
using namespace llvm;
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

LLVM_TYPE transfer2type(std::string& type) {
    std::map<std::string, LLVM_TYPE> &type_trans_table = get_trans_table();
    std::map<std::string, LLVM_TYPE>::iterator it = type_trans_table.find(type);
    if (it != type_trans_table.end()) {
        return it->second;
    } else {
        std::cout << "type is not build-in, please check function type!" << std::endl;
    }
}

void split(std::string &str, std::vector<LLVM_TYPE>& result) {
    std::string::size_type pos;
    str.append(",");  // 扩展字符串以方便操作
    int size = str.size();

    for (int i = 0; i < size; i++) {
        pos = str.find(",", i);
        // LOG(INFO) << "pos=" << pos;
        if (pos < size) {
            std::string s = str.substr(i, pos-i);
            if (s.length() != 0) {
                // LOG(INFO) << "s=" << s;
                result.push_back(transfer2type(s));
            }
            i = pos;
        }
    }
}

void dealFuncType(std::string& str, std::vector<LLVM_TYPE>& func_type) {
    std::string::size_type pos;
    pos = str.find("(");
    if (pos == std::string::npos) {
        std::cout << "function type format error!" << std::endl;
    }
    std::string ret_type_str = str.substr(0, pos);
    // LOG(INFO) << "ret type=" << ret_type_str;
    func_type.push_back(transfer2type(ret_type_str));
    std::string::size_type end_pos = str.find(")");
    if (end_pos == std::string::npos || end_pos <= pos) {
        std::cout << "function type format error!" << std::endl;
    }
    std::string param_type_str = str.substr(pos+1, end_pos - pos - 1);
    // LOG(INFO) << "parap type=" << param_type_str ;

    split(param_type_str, func_type);
    return;
}

enum LLVM_FUNC_ID {
    FUNC_ID_BEGIN = -1,
    FUNC_ADD,
    FUNC_ID_END
};

void regLoadBuildinType() {
    std::map<std::string, LLVM_TYPE> &type_trans_table = get_trans_table();
    type_trans_table.insert(std::make_pair("char", LLVM_CHAR));
    type_trans_table.insert(std::make_pair("bool", LLVM_BOOL));
    type_trans_table.insert(std::make_pair("int", LLVM_INT));
    type_trans_table.insert(std::make_pair("int64", LLVM_INT64));
    type_trans_table.insert(std::make_pair("double", LLVM_DOUBLE));
    type_trans_table.insert(std::make_pair("void", LLVM_VOID));
    type_trans_table.insert(std::make_pair("char*", LLVM_PCHAR));
    type_trans_table.insert(std::make_pair("bool*", LLVM_PBOOL));
    type_trans_table.insert(std::make_pair("int*", LLVM_PINT));
    type_trans_table.insert(std::make_pair("int64*", LLVM_PINT64));
    type_trans_table.insert(std::make_pair("double*", LLVM_PDOUBLE));
    type_trans_table.insert(std::make_pair("void*", LLVM_PVOID));
    type_trans_table.insert(std::make_pair("char&", LLVM_CHAR_REF));
    type_trans_table.insert(std::make_pair("bool&", LLVM_BOOL_REF));
    type_trans_table.insert(std::make_pair("int&", LLVM_INT_REF));
    type_trans_table.insert(std::make_pair("int64&", LLVM_INT64_REF));
    type_trans_table.insert(std::make_pair("double&", LLVM_DOUBLE_REF));
    type_trans_table.insert(std::make_pair("string&", LLVM_STRING_REF));
}

FUNC_INFO* get_system_func_table() {
    static FUNC_INFO system_info_table[FUNC_ID_END];
    return system_info_table;
}
void  regSystemFunc(LLVM_FUNC_ID func_id, std::string func_type, std::string func_name) {
    if (func_id < FUNC_ID_BEGIN || func_id > FUNC_ID_END) {
        std::cout << func_id << "is not found" << std::endl;
        return;
    }

    FUNC_INFO func_info;
    dealFuncType(func_type, func_info.func_type_vec);
    func_info.func_name = func_name;
    get_system_func_table()[func_id] = func_info;
}

int main() {
    regLoadBuildinType();
    regSystemFunc(FUNC_ADD, "int(int,int,void*)", "add");
    for(LLVM_FUNC_ID funid = static_cast<LLVM_FUNC_ID>(FUNC_ID_BEGIN + 1); funid != FUNC_ID_END; funid = static_cast<LLVM_FUNC_ID>(funid + 1)) {
        std::cout << get_system_func_table()[funid].func_name << std::endl;
        for (std::vector<LLVM_TYPE>::iterator it =  get_system_func_table()[funid].func_type_vec.begin(); it != get_system_func_table()[funid].func_type_vec.end(); ++it) {
            std::cout << static_cast<LLVM_TYPE>(*it) << std::endl;
        }
    }
    return 0;
}
