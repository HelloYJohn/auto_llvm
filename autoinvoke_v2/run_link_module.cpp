#include "run_link_module.h"
#include <sstream>
using namespace llvm;

Module* makeLLVMModule(Module* mod, FuncType funcType);
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

void paraListPrint(std::string paraType, void* paraValue) {
    if (paraType == "int") {
        std::cout << "paraListValue: " << *(int*)(paraValue) << std::endl;
    } else if (paraType == "long") {
        std::cout << "paraListValue: " << *(int64_t*)(paraValue) << std::endl;
    } else if (paraType == "double") {
        std::cout << "paraListValue: " << *(double*)(paraValue) << std::endl;
    } else {
        std::cout << "no support" << std::endl;
    }
}
void MiniLlvmEngine::execute(FuncType& funcType) {
    //funcType.ret.pfdsType = "int";
    PFDSType* ret = funcType.ret;
    std::cout << "execute Type: " << (*ret).pfdsType << std::endl; 
    std::cout << "execute Value: " << *((int*)((*ret).pfdsValue)) << std::endl;

    std::vector<PFDSType*> paraList = funcType.paraList;
    for (int i = 0; i < paraList.size(); i++) {
        std::cout << "paraList[" << i << "]Type: " << paraList[i]->pfdsType << std::endl;
        paraListPrint(paraList[i]->pfdsType, paraList[i]->pfdsValue);
        //std::cout << "paraList[" << i << "]Value: " << *((int*)(paraList[i].pfdsValue)) << std::endl;
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
    std::cout << "result is :" << result << std::endl;
    m_ee->freeMachineCodeForFunction(func);
}
Value* getPfdsValueFromPFDSType(Module* mod, BasicBlock* label_entry, Value* ptr_call) {
    ConstantInt* const_int32_18 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt* const_int32_19 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
    PointerType* PointerTy_9 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);
    LoadInst* ptr_32 = new LoadInst(ptr_call, "", false, label_entry);
    std::vector<Value*> ptr_pfdsValue_indices;
    ptr_pfdsValue_indices.push_back(const_int32_19);
    ptr_pfdsValue_indices.push_back(const_int32_18);
    Instruction* ptr_pfdsValue = GetElementPtrInst::Create(ptr_32, ptr_pfdsValue_indices, "pfdsValue", label_entry);
    LoadInst* ptr_33 = new LoadInst(ptr_pfdsValue, "", false, label_entry);
    ptr_33->setAlignment(8);
    CastInst* ptr_34 = new BitCastInst(ptr_33, PointerTy_9, "", label_entry);
    LoadInst* int32_35 = new LoadInst(ptr_34, "", false, label_entry);
    int32_35->setAlignment(4);
    return int32_35;
}
Value* getPFDSTypeFromVector(Module* mod, BasicBlock* label_entry, Value* ptr_paraList, Value* ptr_i) {
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
    PointerType* PointerTy_2 = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

    StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields.push_back(PointerTy_2);
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
    StructType *StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl = mod->getTypeByName("struct.std::_Vector_base<PFDSType *, std::allocator<PFDSType *> >::_Vector_impl");
    if (!StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl) {
        StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl = StructType::create(mod->getContext(), "struct.std::_Vector_base<PFDSType *, std::allocator<PFDSType *> >::_Vector_impl");
    }
    std::vector<Type*>StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields;
    StructType *StructTy_struct_PFDSType = mod->getTypeByName("struct.PFDSType");
    if (!StructTy_struct_PFDSType) {
        StructTy_struct_PFDSType = StructType::create(mod->getContext(), "struct.PFDSType");
    }
    std::vector<Type*>StructTy_struct_PFDSType_fields;
    StructTy_struct_PFDSType_fields.push_back(StructTy_class_std__basic_string);
    StructTy_struct_PFDSType_fields.push_back(PointerTy_2);
    if (StructTy_struct_PFDSType->isOpaque()) {
        StructTy_struct_PFDSType->setBody(StructTy_struct_PFDSType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_4 = PointerType::get(StructTy_struct_PFDSType, 0);

    PointerType* PointerTy_3 = PointerType::get(PointerTy_4, 0);

    StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields.push_back(PointerTy_3);
    StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields.push_back(PointerTy_3);
    StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields.push_back(PointerTy_3);
    if (StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl->isOpaque()) {
        StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl->setBody(StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields, /*isPacked=*/false);
    }

    StructTy_struct_std___Vector_base_fields.push_back(StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl);
    if (StructTy_struct_std___Vector_base->isOpaque()) {
        StructTy_struct_std___Vector_base->setBody(StructTy_struct_std___Vector_base_fields, /*isPacked=*/false);
    }

    StructTy_class_std__vector_fields.push_back(StructTy_struct_std___Vector_base);
    if (StructTy_class_std__vector->isOpaque()) {
        StructTy_class_std__vector->setBody(StructTy_class_std__vector_fields, /*isPacked=*/false);
    }

    StructTy_struct_FuncType_fields.push_back(StructTy_class_std__vector);
    StructTy_struct_FuncType_fields.push_back(PointerTy_4);
    if (StructTy_struct_FuncType->isOpaque()) {
        StructTy_struct_FuncType->setBody(StructTy_struct_FuncType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_1 = PointerType::get(StructTy_struct_FuncType, 0);

    FuncTy_0_args.push_back(PointerTy_1);
    FuncTy_0_args.push_back(PointerTy_2);
    FunctionType* FuncTy_0 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 1),
            /*Params=*/FuncTy_0_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_5 = PointerType::get(PointerTy_1, 0);

    PointerType* PointerTy_6 = PointerType::get(PointerTy_2, 0);

    PointerType* PointerTy_8 = PointerType::get(StructTy_class_std__vector, 0);

    PointerType* PointerTy_7 = PointerType::get(PointerTy_8, 0);

    PointerType* PointerTy_9 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);

    std::vector<Type*>FuncTy_11_args;
    FuncTy_11_args.push_back(PointerTy_8);
    FuncTy_11_args.push_back(IntegerType::get(mod->getContext(), 64));
    FunctionType* FuncTy_11 = FunctionType::get(
            /*Result=*/PointerTy_3,
            /*Params=*/FuncTy_11_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_10 = PointerType::get(FuncTy_11, 0);

    std::vector<Type*>FuncTy_13_args;
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 32));
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 32));
    FuncTy_13_args.push_back(PointerTy_2);
    FunctionType* FuncTy_13 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 32),
            /*Params=*/FuncTy_13_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_12 = PointerType::get(FuncTy_13, 0);

    PointerType* PointerTy_14 = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);

    PointerType* PointerTy_15 = PointerType::get(StructTy_struct_std___Vector_base, 0);

    PointerType* PointerTy_16 = PointerType::get(StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl, 0);

    PointerType* PointerTy_17 = PointerType::get(PointerTy_3, 0);


    // Function Declarations

    Function* func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm = mod->getFunction("_ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm");
    if (!func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm) {
        func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm = Function::Create(
                /*Type=*/FuncTy_11,
                /*Linkage=*/GlobalValue::LinkOnceODRLinkage ,
                /*Name=*/"_ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm", mod); 
        func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm->setCallingConv(CallingConv::C);
        func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm->setAlignment(2);
    }
    AttributeSet func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm_PAL;
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
        func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm_PAL = AttributeSet::get(mod->getContext(), Attrs);

    }
    func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm->setAttributes(func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm_PAL);

    // Global Variable Declarations


    // Constant Definitions
    ConstantInt* const_int32_18 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt* const_int32_19 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
    ConstantInt* const_int32_20 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
    ConstantInt* const_int64_21 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("0"), 10));
    ConstantInt* const_int64_22 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("1"), 10));
    ConstantInt* const_int1_23 = ConstantInt::get(mod->getContext(), APInt(1, StringRef("-1"), 10));
    // Function: _ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm (func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm)
    {
        Function::arg_iterator args = func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm->arg_begin();
        Value* ptr_this = args++;
        ptr_this->setName("this");
        Value* int64___n = args++;
        int64___n->setName("__n");

        BasicBlock* label_entry_51 = BasicBlock::Create(mod->getContext(), "entry",func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm,0);

        // Block entry (label_entry_51)
        AllocaInst* ptr_this_addr = new AllocaInst(PointerTy_8, "this.addr", label_entry_51);
        ptr_this_addr->setAlignment(8);
        AllocaInst* ptr___n_addr = new AllocaInst(IntegerType::get(mod->getContext(), 64), "__n.addr", label_entry_51);
        ptr___n_addr->setAlignment(8);
        StoreInst* void_52 = new StoreInst(ptr_this, ptr_this_addr, false, label_entry_51);
        void_52->setAlignment(8);
        StoreInst* void_53 = new StoreInst(int64___n, ptr___n_addr, false, label_entry_51);
        void_53->setAlignment(8);
        LoadInst* ptr_this1 = new LoadInst(ptr_this_addr, "this1", false, label_entry_51);
        CastInst* ptr_54 = new BitCastInst(ptr_this1, PointerTy_15, "", label_entry_51);
        std::vector<Value*> ptr__M_impl_indices;
        ptr__M_impl_indices.push_back(const_int32_19);
        ptr__M_impl_indices.push_back(const_int32_19);
        Instruction* ptr__M_impl = GetElementPtrInst::Create(ptr_54, ptr__M_impl_indices, "_M_impl", label_entry_51);
        std::vector<Value*> ptr__M_start_indices;
        ptr__M_start_indices.push_back(const_int32_19);
        ptr__M_start_indices.push_back(const_int32_19);
        Instruction* ptr__M_start = GetElementPtrInst::Create(ptr__M_impl, ptr__M_start_indices, "_M_start", label_entry_51);
        LoadInst* ptr_55 = new LoadInst(ptr__M_start, "", false, label_entry_51);
        ptr_55->setAlignment(8);
        LoadInst* int64_56 = new LoadInst(ptr___n_addr, "", false, label_entry_51);
        int64_56->setAlignment(8);
        GetElementPtrInst* ptr_add_ptr = GetElementPtrInst::Create(ptr_55, int64_56, "add.ptr", label_entry_51);
        ReturnInst::Create(mod->getContext(), ptr_add_ptr, label_entry_51);

    }

    LoadInst* ptr_31 = new LoadInst(ptr_paraList, "", false, label_entry);
    ptr_31->setAlignment(8);
    std::vector<Value*> ptr_call_params;
    ptr_call_params.push_back(ptr_31);
    ptr_call_params.push_back(ptr_i);
    CallInst* ptr_call = CallInst::Create(func__ZNSt6vectorIP8PFDSTypeSaIS1_EEixEm, ptr_call_params, "call", label_entry);
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



Module* makeLLVMModule(Module* mod, FuncType funcType) {

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
    PointerType* PointerTy_2 = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

    StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields.push_back(PointerTy_2);
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
    StructType *StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl = mod->getTypeByName("struct.std::_Vector_base<PFDSType *, std::allocator<PFDSType *> >::_Vector_impl");
    if (!StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl) {
        StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl = StructType::create(mod->getContext(), "struct.std::_Vector_base<PFDSType *, std::allocator<PFDSType *> >::_Vector_impl");
    }
    std::vector<Type*>StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields;
    StructType *StructTy_struct_PFDSType = mod->getTypeByName("struct.PFDSType");
    if (!StructTy_struct_PFDSType) {
        StructTy_struct_PFDSType = StructType::create(mod->getContext(), "struct.PFDSType");
    }
    std::vector<Type*>StructTy_struct_PFDSType_fields;
    StructTy_struct_PFDSType_fields.push_back(StructTy_class_std__basic_string);
    StructTy_struct_PFDSType_fields.push_back(PointerTy_2);
    if (StructTy_struct_PFDSType->isOpaque()) {
        StructTy_struct_PFDSType->setBody(StructTy_struct_PFDSType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_4 = PointerType::get(StructTy_struct_PFDSType, 0);

    PointerType* PointerTy_3 = PointerType::get(PointerTy_4, 0);

    StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields.push_back(PointerTy_3);
    StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields.push_back(PointerTy_3);
    StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields.push_back(PointerTy_3);
    if (StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl->isOpaque()) {
        StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl->setBody(StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl_fields, /*isPacked=*/false);
    }

    StructTy_struct_std___Vector_base_fields.push_back(StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl);
    if (StructTy_struct_std___Vector_base->isOpaque()) {
        StructTy_struct_std___Vector_base->setBody(StructTy_struct_std___Vector_base_fields, /*isPacked=*/false);
    }

    StructTy_class_std__vector_fields.push_back(StructTy_struct_std___Vector_base);
    if (StructTy_class_std__vector->isOpaque()) {
        StructTy_class_std__vector->setBody(StructTy_class_std__vector_fields, /*isPacked=*/false);
    }

    StructTy_struct_FuncType_fields.push_back(StructTy_class_std__vector);
    StructTy_struct_FuncType_fields.push_back(PointerTy_4);
    if (StructTy_struct_FuncType->isOpaque()) {
        StructTy_struct_FuncType->setBody(StructTy_struct_FuncType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_1 = PointerType::get(StructTy_struct_FuncType, 0);

    FuncTy_0_args.push_back(PointerTy_1);
    FuncTy_0_args.push_back(PointerTy_2);
    FunctionType* FuncTy_0 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 1),
            /*Params=*/FuncTy_0_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_5 = PointerType::get(PointerTy_1, 0);

    PointerType* PointerTy_6 = PointerType::get(PointerTy_2, 0);

    PointerType* PointerTy_8 = PointerType::get(StructTy_class_std__vector, 0);

    PointerType* PointerTy_7 = PointerType::get(PointerTy_8, 0);

    PointerType* PointerTy_9 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);

    std::vector<Type*>FuncTy_11_args;
    FuncTy_11_args.push_back(PointerTy_8);
    FuncTy_11_args.push_back(IntegerType::get(mod->getContext(), 64));
    FunctionType* FuncTy_11 = FunctionType::get(
            /*Result=*/PointerTy_3,
            /*Params=*/FuncTy_11_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_10 = PointerType::get(FuncTy_11, 0);

    std::vector<Type*>FuncTy_13_args;
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 32));
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 32));
    FuncTy_13_args.push_back(PointerTy_2);
    FunctionType* FuncTy_13 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 32),
            /*Params=*/FuncTy_13_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_12 = PointerType::get(FuncTy_13, 0);

    PointerType* PointerTy_14 = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);

    PointerType* PointerTy_15 = PointerType::get(StructTy_struct_std___Vector_base, 0);

    PointerType* PointerTy_16 = PointerType::get(StructTy_struct_std___Vector_base_PFDSType____std__allocator_PFDSType________Vector_impl, 0);

    PointerType* PointerTy_17 = PointerType::get(PointerTy_3, 0);


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



    // Global Variable Declarations


    // Constant Definitions
    ConstantInt* const_int32_18 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt* const_int32_19 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));
    ConstantInt* const_int32_20 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
    ConstantInt* const_int64_21 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("0"), 10));
    ConstantInt* const_int64_22 = ConstantInt::get(mod->getContext(), APInt(64, StringRef("1"), 10));
    ConstantInt* const_int1_23 = ConstantInt::get(mod->getContext(), APInt(1, StringRef("-1"), 10));

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
        AllocaInst* ptr_funcT_addr = new AllocaInst(PointerTy_1, "funcT.addr", label_entry);
        ptr_funcT_addr->setAlignment(8);
        AllocaInst* ptr_dl_handle_addr = new AllocaInst(PointerTy_2, "dl_handle.addr", label_entry);
        ptr_dl_handle_addr->setAlignment(8);
        AllocaInst* ptr_paraList = new AllocaInst(PointerTy_8, "paraList", label_entry);
        ptr_paraList->setAlignment(8);
        AllocaInst* ptr_ret = new AllocaInst(PointerTy_4, "ret", label_entry);
        ptr_ret->setAlignment(8);
        StoreInst* void_24 = new StoreInst(ptr_funcT, ptr_funcT_addr, false, label_entry);
        void_24->setAlignment(8);
        StoreInst* void_25 = new StoreInst(ptr_dl_handle, ptr_dl_handle_addr, false, label_entry);
        void_25->setAlignment(8);
        LoadInst* ptr_26 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_26->setAlignment(8);
        std::vector<Value*> ptr_paraList1_indices;
        ptr_paraList1_indices.push_back(const_int32_19);
        ptr_paraList1_indices.push_back(const_int32_18);
        Instruction* ptr_paraList1 = GetElementPtrInst::Create(ptr_26, ptr_paraList1_indices, "paraList1", label_entry);
        StoreInst* void_27 = new StoreInst(ptr_paraList1, ptr_paraList, false, label_entry);
        void_27->setAlignment(8);
        LoadInst* ptr_28 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_28->setAlignment(8);
        std::vector<Value*> ptr_ret2_indices;
        ptr_ret2_indices.push_back(const_int32_19);
        ptr_ret2_indices.push_back(const_int32_20);
        Instruction* ptr_ret2 = GetElementPtrInst::Create(ptr_28, ptr_ret2_indices, "ret2", label_entry);
        LoadInst* ptr_29 = new LoadInst(ptr_ret2, "", false, label_entry);
        ptr_29->setAlignment(8);
        StoreInst* void_30 = new StoreInst(ptr_29, ptr_ret, false, label_entry);
        void_30->setAlignment(8);
/*
        Value* ptr_call = getPFDSTypeFromVector(mod, label_entry, ptr_paraList, const_int64_21);

        Value* int32_35 = getPfdsValueFromPFDSType(mod, label_entry, ptr_call);

        Value* ptr_call3 = getPFDSTypeFromVector(mod, label_entry, ptr_paraList, const_int64_22);

        Value* int32_41 = getPfdsValueFromPFDSType(mod, label_entry, ptr_call3);
*/
        Function* func_add = mod->getFunction(funcType.funcName.c_str());
        LoadInst* ptr_45 = new LoadInst(ptr_dl_handle_addr, "", false, label_entry);
        ptr_45->setAlignment(8);
        std::vector<Value*> int32_call5_params;
        for (int i = 0; i < funcType.paraList.size(); i++) {
            putParaIntoVec(mod, label_entry, ptr_paraList, int32_call5_params, i);
        }
        int32_call5_params.push_back(ptr_45);
        CallInst* int32_call5 = CallInst::Create(func_add, int32_call5_params, "call5", label_entry);
        int32_call5->setCallingConv(CallingConv::C);
        int32_call5->setTailCall(false);
        AttributeSet int32_call5_PAL;
        int32_call5->setAttributes(int32_call5_PAL);

        LoadInst* ptr_46 = new LoadInst(ptr_ret, "", false, label_entry);
        ptr_46->setAlignment(8);
        std::vector<Value*> ptr_pfdsValue6_indices;
        ptr_pfdsValue6_indices.push_back(const_int32_19);
        ptr_pfdsValue6_indices.push_back(const_int32_18);
        Instruction* ptr_pfdsValue6 = GetElementPtrInst::Create(ptr_46, ptr_pfdsValue6_indices, "pfdsValue6", label_entry);
        LoadInst* ptr_47 = new LoadInst(ptr_pfdsValue6, "", false, label_entry);
        ptr_47->setAlignment(8);
        CastInst* ptr_48 = new BitCastInst(ptr_47, PointerTy_9, "", label_entry);
        StoreInst* void_49 = new StoreInst(int32_call5, ptr_48, false, label_entry);
        void_49->setAlignment(4);
        ReturnInst::Create(mod->getContext(), const_int1_23, label_entry);

    }


    return mod;
}
