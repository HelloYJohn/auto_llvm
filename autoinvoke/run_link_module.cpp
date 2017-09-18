#include "run_link_module.h"
#include <sstream>
using namespace llvm;

Module* makeLLVMModule(Module* mod, FuncType& funcType);
typedef bool (*invoke)(FuncType& funcT, void* dl_handle);

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


void MiniLlvmEngine::init(FuncType& funcType)
{
    m_so = load_shared_lib();
    llvm_start_multithreaded();
    InitializeNativeTarget();

    SMDiagnostic error;
    Module* mod = ParseIRFile("load_cpp_bc.bc", error, *m_llvm_context);

    std::string link_error_code = "";	
    mod = makeLLVMModule(mod, funcType);
    Linker::LinkModules(m_mod , mod , 1 , &link_error_code) ;

    std::string error_code = "";
    EngineBuilder eb = EngineBuilder(m_mod);
    //EngineBuilder eb = EngineBuilder(mod);
    eb.setEngineKind(EngineKind::JIT);
    eb.setErrorStr(&error_code);
    m_ee = eb.create();
}

void valuePrint(std::string paraType, void* paraValue) {
    if (paraType == "int") {
        std::cout << "Value: " << *(int*)(paraValue) << std::endl;
    } else if (paraType == "long") {
        std::cout << "Value: " << *(int64_t*)(paraValue) << std::endl;
    } else if (paraType == "double") {
        std::cout << "Value: " << *(double*)(paraValue) << std::endl;
    } else {
        std::cout << "no support" << std::endl;
    }
}
void MiniLlvmEngine::execute(FuncType& funcType, JniStatus status) {
    //funcType.ret.pfdsType = "int";
    PFDSType* ret = funcType.ret;
    std::cout << "result Type: " << (*ret).pfdsType << std::endl; 
    valuePrint((*ret).pfdsType, (*ret).pfdsValue);

    std::vector<PFDSType>* paraList = funcType.paraList;
    for (int i = 0; i < (*paraList).size(); i++) {
        std::cout << "paraList[" << i << "]Type: " << (*paraList)[i].pfdsType << std::endl;
        valuePrint((*paraList)[i].pfdsType, (*paraList)[i].pfdsValue);
        //std::cout << "paraList[" << i << "]Value: " << *((int*)((*paraList)[i].pfdsValue)) << std::endl;
    }

    Function* func = NULL;
    func =  m_mod->getFunction("invoke");
    if( func == NULL)
    {
        std::cout<< "get function error.." << std::endl;
        return;
    }

    void* jitAddr = m_ee->getPointerToFunction(func);
    invoke jitFunc = reinterpret_cast<invoke>(jitAddr);
    bool result = jitFunc(funcType, m_so);
    std::cout << "result is :" << (result ? "true" : "false") << std::endl;
    m_ee->freeMachineCodeForFunction(func);
}
Value* getPfdsValueFromPFDSType(Module* mod, BasicBlock* label_entry, Value* ptr_call) {

    PointerType* PointerTy_int32 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);
    ConstantInt* const_int32_0 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
    ConstantInt* const_int32_1 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    std::vector<Value*> ptr_pfdsValue_indices;
    ptr_pfdsValue_indices.push_back(const_int32_0);
    ptr_pfdsValue_indices.push_back(const_int32_1);
    Instruction* ptr_pfdsValue = GetElementPtrInst::Create(ptr_call, ptr_pfdsValue_indices, "pfdsValue", label_entry);
    LoadInst* ptr_36 = new LoadInst(ptr_pfdsValue, "", false, label_entry);
    ptr_36->setAlignment(8);
    CastInst* ptr_37 = new BitCastInst(ptr_36, PointerTy_int32, "", label_entry);
    LoadInst* int32_38 = new LoadInst(ptr_37, "", false, label_entry);
    int32_38->setAlignment(4);
    return int32_38;
}
Value* getPFDSTypeFromVector(Module* mod, BasicBlock* label_entry, Value* ptr_paraList, Value* ptr_i) {
    std::vector<Type*>FuncTy_0_args;
    StructType *StructTy_struct_FuncType = mod->getTypeByName("struct.FuncType");
    if (!StructTy_struct_FuncType) {
        StructTy_struct_FuncType = StructType::create(mod->getContext(), "struct.FuncType");
    }
    std::vector<Type*>StructTy_struct_FuncType_fields;
    StructType *StructTy_class_std__basic_string = mod->getTypeByName("class.std::basic_string");
    if (!StructTy_class_std__basic_string) {
        StructTy_class_std__basic_string = StructType::create(mod->getContext(), "class.std::basic_string");
    }
    std::vector<Type*>StructTy_class_std__basic_string_fields;
    StructType *StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider = mod->getTypeByName("struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider");
    if (!StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider) {
        StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider = StructType::create(mod->getContext(), "struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider");
    }
    std::vector<Type*>StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields;
    PointerType* PointerTy_Void = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

    StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields.push_back(PointerTy_Void);
    if (StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider->isOpaque()) {
        StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider->setBody(StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields, /*isPacked=*/false);
    }

    StructTy_class_std__basic_string_fields.push_back(StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider);
    if (StructTy_class_std__basic_string->isOpaque()) {
        StructTy_class_std__basic_string->setBody(StructTy_class_std__basic_string_fields, /*isPacked=*/false);
    }

    StructTy_struct_FuncType_fields.push_back(StructTy_class_std__basic_string);
    StructType *StructTy_class_std__vector = mod->getTypeByName("class.std::vector");
    if (!StructTy_class_std__vector) {
        StructTy_class_std__vector = StructType::create(mod->getContext(), "class.std::vector");
    }
    std::vector<Type*>StructTy_class_std__vector_fields;
    StructType *StructTy_struct_std___Vector_base = mod->getTypeByName("struct.std::_Vector_base");
    if (!StructTy_struct_std___Vector_base) {
        StructTy_struct_std___Vector_base = StructType::create(mod->getContext(), "struct.std::_Vector_base");
    }
    std::vector<Type*>StructTy_struct_std___Vector_base_fields;
    StructType *StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl = mod->getTypeByName("struct.std::_Vector_base<PFDSType, std::allocator<PFDSType> >::_Vector_impl");
    if (!StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl) {
        StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl = StructType::create(mod->getContext(), "struct.std::_Vector_base<PFDSType, std::allocator<PFDSType> >::_Vector_impl");
    }
    std::vector<Type*>StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields;
    StructType *StructTy_struct_PFDSType = mod->getTypeByName("struct.PFDSType");
    if (!StructTy_struct_PFDSType) {
        StructTy_struct_PFDSType = StructType::create(mod->getContext(), "struct.PFDSType");
    }
    std::vector<Type*>StructTy_struct_PFDSType_fields;
    StructTy_struct_PFDSType_fields.push_back(StructTy_class_std__basic_string);
    StructTy_struct_PFDSType_fields.push_back(PointerTy_Void);
    if (StructTy_struct_PFDSType->isOpaque()) {
        StructTy_struct_PFDSType->setBody(StructTy_struct_PFDSType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_PFDSType = PointerType::get(StructTy_struct_PFDSType, 0);

    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_PFDSType);
    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_PFDSType);
    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_PFDSType);
    if (StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl->isOpaque()) {
        StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl->setBody(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields, /*isPacked=*/false);
    }

    StructTy_struct_std___Vector_base_fields.push_back(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl);
    if (StructTy_struct_std___Vector_base->isOpaque()) {
        StructTy_struct_std___Vector_base->setBody(StructTy_struct_std___Vector_base_fields, /*isPacked=*/false);
    }

    StructTy_class_std__vector_fields.push_back(StructTy_struct_std___Vector_base);
    if (StructTy_class_std__vector->isOpaque()) {
        StructTy_class_std__vector->setBody(StructTy_class_std__vector_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_Vector = PointerType::get(StructTy_class_std__vector, 0);

    StructTy_struct_FuncType_fields.push_back(PointerTy_Vector);
    StructTy_struct_FuncType_fields.push_back(PointerTy_PFDSType);
    if (StructTy_struct_FuncType->isOpaque()) {
        StructTy_struct_FuncType->setBody(StructTy_struct_FuncType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_FuncType = PointerType::get(StructTy_struct_FuncType, 0);

    FuncTy_0_args.push_back(PointerTy_FuncType);
    FuncTy_0_args.push_back(PointerTy_Void);
    FunctionType* FuncTy_0 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 1),
            /*Params=*/FuncTy_0_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_string = PointerType::get(StructTy_class_std__basic_string, 0);

    PointerType* PointerTy_int32 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);

    std::vector<Type*>FuncTy_13_args;
    FuncTy_13_args.push_back(PointerTy_Vector);
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 64));
    FunctionType* FuncTy_13 = FunctionType::get(
            /*Result=*/PointerTy_PFDSType,
            /*Params=*/FuncTy_13_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_12 = PointerType::get(FuncTy_13, 0);

    PointerType* PointerTy_16 = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);

    PointerType* PointerTy_17 = PointerType::get(StructTy_struct_std___Vector_base, 0);

    PointerType* PointerTy_18 = PointerType::get(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl, 0);


    // Function Declarations

    Function* func_invoke = mod->getFunction("invoke");

    Function* func_vectorPFDSType_at = mod->getFunction("_ZNSt6vectorI8PFDSTypeSaIS0_EEixEm");
    if (!func_vectorPFDSType_at) {
        func_vectorPFDSType_at = Function::Create(
                /*Type=*/FuncTy_13,
                /*Linkage=*/GlobalValue::LinkOnceODRLinkage ,
                /*Name=*/"_ZNSt6vectorI8PFDSTypeSaIS0_EEixEm", mod); 
        func_vectorPFDSType_at->setCallingConv(CallingConv::C);
        func_vectorPFDSType_at->setAlignment(2);
    }
    AttributeSet func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm_PAL;
    {
        SmallVector<AttributeSet, 4> Attrs;
        AttributeSet PAS;
        {
            AttrBuilder B;
            B.addAttribute(Attribute::NoUnwind);
            B.addAttribute(Attribute::UWTable);
            PAS = AttributeSet::get(mod->getContext(), ~0U, B);
        }

        Attrs.push_back(PAS);
        func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm_PAL = AttributeSet::get(mod->getContext(), Attrs);

    }
    func_vectorPFDSType_at->setAttributes(func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm_PAL);

    // Constant Definitions
    ConstantInt* const_int32_1 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt* const_int32_0 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
    ConstantInt* const_int32_2 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
    ConstantInt* const_int64_0 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("0"), 10));
    ConstantInt* const_int64_1 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("1"), 10));
    ConstantInt* const_int1__1 = ConstantInt::get(mod->getContext(), APInt(1, StringRef("-1"), 10));
    // Function: _ZNSt6vectorI8PFDSTypeSaIS0_EEixEm (func_vectorPFDSType_at)
    {
        Function::arg_iterator args = func_vectorPFDSType_at->arg_begin();
        Value* ptr_this = args++;
        ptr_this->setName("this");
        Value* int64___n = args++;
        int64___n->setName("__n");

        BasicBlock* label_entry_53 = BasicBlock::Create(mod->getContext(), "entry",func_vectorPFDSType_at,0);

        // Block entry (label_entry_53)
        AllocaInst* ptr_this_addr = new AllocaInst(PointerTy_Vector, "this.addr", label_entry_53);
        ptr_this_addr->setAlignment(8);
        AllocaInst* ptr___n_addr = new AllocaInst(IntegerType::get(mod->getContext(), 64), "__n.addr", label_entry_53);
        ptr___n_addr->setAlignment(8);
        StoreInst* void_54 = new StoreInst(ptr_this, ptr_this_addr, false, label_entry_53);
        void_54->setAlignment(8);
        StoreInst* void_55 = new StoreInst(int64___n, ptr___n_addr, false, label_entry_53);
        void_55->setAlignment(8);
        LoadInst* ptr_this1 = new LoadInst(ptr_this_addr, "this1", false, label_entry_53);
        CastInst* ptr_56 = new BitCastInst(ptr_this1, PointerTy_17, "", label_entry_53);
        std::vector<Value*> ptr__M_impl_indices;
        ptr__M_impl_indices.push_back(const_int32_0);
        ptr__M_impl_indices.push_back(const_int32_0);
        Instruction* ptr__M_impl = GetElementPtrInst::Create(ptr_56, ptr__M_impl_indices, "_M_impl", label_entry_53);
        std::vector<Value*> ptr__M_start_indices;
        ptr__M_start_indices.push_back(const_int32_0);
        ptr__M_start_indices.push_back(const_int32_0);
        Instruction* ptr__M_start = GetElementPtrInst::Create(ptr__M_impl, ptr__M_start_indices, "_M_start", label_entry_53);
        LoadInst* ptr_57 = new LoadInst(ptr__M_start, "", false, label_entry_53);
        ptr_57->setAlignment(8);
        LoadInst* int64_58 = new LoadInst(ptr___n_addr, "", false, label_entry_53);
        int64_58->setAlignment(8);
        GetElementPtrInst* ptr_add_ptr = GetElementPtrInst::Create(ptr_57, int64_58, "add.ptr", label_entry_53);
        ReturnInst::Create(mod->getContext(), ptr_add_ptr, label_entry_53);

    }
    LoadInst* ptr_35 = new LoadInst(ptr_paraList, "", false, label_entry);
    ptr_35->setAlignment(8);
    std::vector<Value*> ptr_call_params;
    ptr_call_params.push_back(ptr_35);
    ptr_call_params.push_back(ptr_i);
    CallInst* ptr_call = CallInst::Create(func_vectorPFDSType_at, ptr_call_params, "call", label_entry);
    ptr_call->setCallingConv(CallingConv::C);
    ptr_call->setTailCall(false);
    AttributeSet ptr_call_PAL;
    ptr_call->setAttributes(ptr_call_PAL);

    return ptr_call;
}
void putParaIntoVec(Module *mod, BasicBlock* label_entry, Value* ptr_paraList, std::vector<Value*>& int32_call6_params, int i) {
    std::stringstream ss;
    ss << i;
    ConstantInt* const_int64_i = ConstantInt::get(mod->getContext(), APInt(64, StringRef(ss.str().c_str()), 10));
    // get PFDSType struct function
    Value* ptr_call =  getPFDSTypeFromVector(mod, label_entry, ptr_paraList, const_int64_i);
    // get pfds_value from PFDSType
    Value* int32_38 = getPfdsValueFromPFDSType(mod, label_entry, ptr_call);
    int32_call6_params.push_back(int32_38);                
}

void writeResult(Module* mod, BasicBlock* label_entry, Value* ptr_result, Value* result_addr) {
    PointerType* PointerTy_int32 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);
    CastInst* ptr_50 = new BitCastInst(result_addr, PointerTy_int32, "", label_entry);
    StoreInst* void_51 = new StoreInst(ptr_result, ptr_50, false, label_entry);
    void_51->setAlignment(4);

}
Module* makeLLVMModule(Module* mod, FuncType& funcType) {
    // Module Construction
    mod->setDataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128");
    mod->setTargetTriple("x86_64-unknown-linux-gnu");

    // Type Definitions
    std::vector<Type*>FuncTy_0_args;
    StructType *StructTy_struct_FuncType = mod->getTypeByName("struct.FuncType");
    if (!StructTy_struct_FuncType) {
        StructTy_struct_FuncType = StructType::create(mod->getContext(), "struct.FuncType");
    }
    std::vector<Type*>StructTy_struct_FuncType_fields;
    StructType *StructTy_class_std__basic_string = mod->getTypeByName("class.std::basic_string");
    if (!StructTy_class_std__basic_string) {
        StructTy_class_std__basic_string = StructType::create(mod->getContext(), "class.std::basic_string");
    }
    std::vector<Type*>StructTy_class_std__basic_string_fields;
    StructType *StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider = mod->getTypeByName("struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider");
    if (!StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider) {
        StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider = StructType::create(mod->getContext(), "struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider");
    }
    std::vector<Type*>StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields;
    PointerType* PointerTy_Void = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

    StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields.push_back(PointerTy_Void);
    if (StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider->isOpaque()) {
        StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider->setBody(StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields, /*isPacked=*/false);
    }

    StructTy_class_std__basic_string_fields.push_back(StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider);
    if (StructTy_class_std__basic_string->isOpaque()) {
        StructTy_class_std__basic_string->setBody(StructTy_class_std__basic_string_fields, /*isPacked=*/false);
    }

    StructTy_struct_FuncType_fields.push_back(StructTy_class_std__basic_string);
    StructType *StructTy_class_std__vector = mod->getTypeByName("class.std::vector");
    if (!StructTy_class_std__vector) {
        StructTy_class_std__vector = StructType::create(mod->getContext(), "class.std::vector");
    }
    std::vector<Type*>StructTy_class_std__vector_fields;
    StructType *StructTy_struct_std___Vector_base = mod->getTypeByName("struct.std::_Vector_base");
    if (!StructTy_struct_std___Vector_base) {
        StructTy_struct_std___Vector_base = StructType::create(mod->getContext(), "struct.std::_Vector_base");
    }
    std::vector<Type*>StructTy_struct_std___Vector_base_fields;
    StructType *StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl = mod->getTypeByName("struct.std::_Vector_base<PFDSType, std::allocator<PFDSType> >::_Vector_impl");
    if (!StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl) {
        StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl = StructType::create(mod->getContext(), "struct.std::_Vector_base<PFDSType, std::allocator<PFDSType> >::_Vector_impl");
    }
    std::vector<Type*>StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields;
    StructType *StructTy_struct_PFDSType = mod->getTypeByName("struct.PFDSType");
    if (!StructTy_struct_PFDSType) {
        StructTy_struct_PFDSType = StructType::create(mod->getContext(), "struct.PFDSType");
    }
    std::vector<Type*>StructTy_struct_PFDSType_fields;
    StructTy_struct_PFDSType_fields.push_back(StructTy_class_std__basic_string);
    StructTy_struct_PFDSType_fields.push_back(PointerTy_Void);
    if (StructTy_struct_PFDSType->isOpaque()) {
        StructTy_struct_PFDSType->setBody(StructTy_struct_PFDSType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_PFDSType = PointerType::get(StructTy_struct_PFDSType, 0);

    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_PFDSType);
    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_PFDSType);
    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_PFDSType);
    if (StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl->isOpaque()) {
        StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl->setBody(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields, /*isPacked=*/false);
    }

    StructTy_struct_std___Vector_base_fields.push_back(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl);
    if (StructTy_struct_std___Vector_base->isOpaque()) {
        StructTy_struct_std___Vector_base->setBody(StructTy_struct_std___Vector_base_fields, /*isPacked=*/false);
    }

    StructTy_class_std__vector_fields.push_back(StructTy_struct_std___Vector_base);
    if (StructTy_class_std__vector->isOpaque()) {
        StructTy_class_std__vector->setBody(StructTy_class_std__vector_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_Vector = PointerType::get(StructTy_class_std__vector, 0);

    StructTy_struct_FuncType_fields.push_back(PointerTy_Vector);
    StructTy_struct_FuncType_fields.push_back(PointerTy_PFDSType);
    if (StructTy_struct_FuncType->isOpaque()) {
        StructTy_struct_FuncType->setBody(StructTy_struct_FuncType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_FuncType = PointerType::get(StructTy_struct_FuncType, 0);

    FuncTy_0_args.push_back(PointerTy_FuncType);
    FuncTy_0_args.push_back(PointerTy_Void);
    FunctionType* FuncTy_0 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 1),
            /*Params=*/FuncTy_0_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_string = PointerType::get(StructTy_class_std__basic_string, 0);

    PointerType* PointerTy_int32 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);

    std::vector<Type*>FuncTy_13_args;
    FuncTy_13_args.push_back(PointerTy_Vector);
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 64));
    FunctionType* FuncTy_13 = FunctionType::get(
            /*Result=*/PointerTy_PFDSType,
            /*Params=*/FuncTy_13_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_12 = PointerType::get(FuncTy_13, 0);

    PointerType* PointerTy_16 = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);

    PointerType* PointerTy_17 = PointerType::get(StructTy_struct_std___Vector_base, 0);

    PointerType* PointerTy_18 = PointerType::get(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl, 0);


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


    // Constant Definitions
    ConstantInt* const_int32_1 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt* const_int32_0 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
    ConstantInt* const_int32_2 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
    ConstantInt* const_int64_0 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("0"), 10));
    ConstantInt* const_int64_1 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("1"), 10));
    ConstantInt* const_int1__1 = ConstantInt::get(mod->getContext(), APInt(1, StringRef("-1"), 10));

    // Global Variable Definitions

    // Function Definitions

    // Function: invoke (func_invoke)
    {
        Function::arg_iterator args = func_invoke->arg_begin();
        Value* ptr_funcT = args++;
        ptr_funcT->setName("funcT");
        Value* ptr_dl_handle = args++;
        ptr_dl_handle->setName("dl_handle");

        BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",func_invoke,0);

        // Block entry (label_entry)
        // alloc Structure used and point the addr

        AllocaInst* ptr_funcT_addr = new AllocaInst(PointerTy_FuncType, "funcT.addr", label_entry);
        ptr_funcT_addr->setAlignment(8);
        AllocaInst* ptr_dl_handle_addr = new AllocaInst(PointerTy_Void, "dl_handle.addr", label_entry);
        ptr_dl_handle_addr->setAlignment(8);
        AllocaInst* ptr_paraList = new AllocaInst(PointerTy_Vector, "paraList", label_entry);
        ptr_paraList->setAlignment(8);
        AllocaInst* ptr_ret = new AllocaInst(PointerTy_PFDSType, "ret", label_entry);
        ptr_ret->setAlignment(8);

        StoreInst* void_25 = new StoreInst(ptr_funcT, ptr_funcT_addr, false, label_entry);
        void_25->setAlignment(8);
        StoreInst* void_26 = new StoreInst(ptr_dl_handle, ptr_dl_handle_addr, false, label_entry);
        void_26->setAlignment(8);
        LoadInst* ptr_27 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_27->setAlignment(8);
        LoadInst* ptr_29 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_29->setAlignment(8);
        std::vector<Value*> ptr_paraList2_indices;
        ptr_paraList2_indices.push_back(const_int32_0);
        ptr_paraList2_indices.push_back(const_int32_1);
        Instruction* ptr_paraList2 = GetElementPtrInst::Create(ptr_29, ptr_paraList2_indices, "paraList2", label_entry);
        LoadInst* ptr_30 = new LoadInst(ptr_paraList2, "", false, label_entry);
        ptr_30->setAlignment(8);
        StoreInst* void_31 = new StoreInst(ptr_30, ptr_paraList, false, label_entry);
        void_31->setAlignment(8);
        LoadInst* ptr_32 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_32->setAlignment(8);
        std::vector<Value*> ptr_ret3_indices;
        ptr_ret3_indices.push_back(const_int32_0);
        ptr_ret3_indices.push_back(const_int32_2);
        Instruction* ptr_ret3 = GetElementPtrInst::Create(ptr_32, ptr_ret3_indices, "ret3", label_entry);
        LoadInst* ptr_33 = new LoadInst(ptr_ret3, "", false, label_entry);
        ptr_33->setAlignment(8);
        StoreInst* void_34 = new StoreInst(ptr_33, ptr_ret, false, label_entry);
        void_34->setAlignment(8);

        Function* func_add = mod->getFunction(funcType.funcName.c_str());
        LoadInst* ptr_47 = new LoadInst(ptr_dl_handle_addr, "", false, label_entry);
        ptr_47->setAlignment(8);
        std::vector<Value*> int32_call6_params;
        // put para into vector
        for (int i = 0; i < funcType.paraList->size(); i++) {
            putParaIntoVec(mod, label_entry, ptr_paraList, int32_call6_params, i);
        }
        int32_call6_params.push_back(ptr_47);
        CallInst* int32_call6 = CallInst::Create(func_add, int32_call6_params, "call6", label_entry);
        int32_call6->setCallingConv(CallingConv::C);
        int32_call6->setTailCall(false);
        AttributeSet int32_call6_PAL;
        int32_call6->setAttributes(int32_call6_PAL);

        LoadInst* ptr_48 = new LoadInst(ptr_ret, "", false, label_entry);
        ptr_48->setAlignment(8);
        std::vector<Value*> ptr_pfdsValue7_indices;
        ptr_pfdsValue7_indices.push_back(const_int32_0);
        ptr_pfdsValue7_indices.push_back(const_int32_1);
        Instruction* ptr_pfdsValue7 = GetElementPtrInst::Create(ptr_48, ptr_pfdsValue7_indices, "pfdsValue7", label_entry);
        LoadInst* ptr_49 = new LoadInst(ptr_pfdsValue7, "", false, label_entry);
        ptr_49->setAlignment(8);
        // write result
        writeResult(mod, label_entry, int32_call6, ptr_49);
        ReturnInst::Create(mod->getContext(), const_int1__1, label_entry);

    }
    return mod;
}
