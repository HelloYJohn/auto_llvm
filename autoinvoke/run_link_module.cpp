#include "run_link_module.h"
using namespace llvm;

Module* makeLLVMModule(Module* mod);
typedef int (*fnGetCalcResult)(int a, int b, void* dl_handle);
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


void MiniLlvmEngine::init()
{
    m_so = load_shared_lib();
    llvm_start_multithreaded();
    InitializeNativeTarget();

    SMDiagnostic error;
    Module* mod = ParseIRFile("load_cpp_bc.bc", error, *m_llvm_context);

    std::string link_error_code = "";	
    mod = makeLLVMModule(mod);
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

    std::vector<PFDSType>* paraList = funcType.paraList;
    for (int i = 0; i < (*paraList).size(); i++) {
        std::cout << "paraList[" << i << "]Type: " << (*paraList)[i].pfdsType << std::endl;
        paraListPrint((*paraList)[i].pfdsType, (*paraList)[i].pfdsValue);
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
    std::cout << "result is :" << result << std::endl;
    m_ee->freeMachineCodeForFunction(func);
}

Module* makeLLVMModule(Module* mod) {
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
    StructTy_struct_FuncType_fields.push_back(IntegerType::get(mod->getContext(), 64));
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
    PointerType* PointerTy_4 = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);

    StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields.push_back(PointerTy_4);
    if (StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider->isOpaque()) {
        StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider->setBody(StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider_fields, /*isPacked=*/false);
    }

    StructTy_class_std__basic_string_fields.push_back(StructTy_struct_std__basic_string_char__std__char_traits_char___std__allocator_char______Alloc_hider);
    if (StructTy_class_std__basic_string->isOpaque()) {
        StructTy_class_std__basic_string->setBody(StructTy_class_std__basic_string_fields, /*isPacked=*/false);
    }

    StructTy_struct_PFDSType_fields.push_back(StructTy_class_std__basic_string);
    StructTy_struct_PFDSType_fields.push_back(PointerTy_4);
    if (StructTy_struct_PFDSType->isOpaque()) {
        StructTy_struct_PFDSType->setBody(StructTy_struct_PFDSType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_3 = PointerType::get(StructTy_struct_PFDSType, 0);

    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_3);
    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_3);
    StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl_fields.push_back(PointerTy_3);
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

    PointerType* PointerTy_2 = PointerType::get(StructTy_class_std__vector, 0);

    StructTy_struct_FuncType_fields.push_back(PointerTy_2);
    StructTy_struct_FuncType_fields.push_back(PointerTy_3);
    if (StructTy_struct_FuncType->isOpaque()) {
        StructTy_struct_FuncType->setBody(StructTy_struct_FuncType_fields, /*isPacked=*/false);
    }

    PointerType* PointerTy_1 = PointerType::get(StructTy_struct_FuncType, 0);

    FuncTy_0_args.push_back(PointerTy_1);
    FuncTy_0_args.push_back(PointerTy_4);
    FunctionType* FuncTy_0 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 1),
            /*Params=*/FuncTy_0_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_5 = PointerType::get(PointerTy_1, 0);

    PointerType* PointerTy_6 = PointerType::get(PointerTy_4, 0);

    PointerType* PointerTy_8 = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);

    PointerType* PointerTy_7 = PointerType::get(PointerTy_8, 0);

    PointerType* PointerTy_9 = PointerType::get(PointerTy_2, 0);

    PointerType* PointerTy_10 = PointerType::get(PointerTy_3, 0);

    PointerType* PointerTy_11 = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);

    std::vector<Type*>FuncTy_13_args;
    FuncTy_13_args.push_back(PointerTy_2);
    FuncTy_13_args.push_back(IntegerType::get(mod->getContext(), 64));
    FunctionType* FuncTy_13 = FunctionType::get(
            /*Result=*/PointerTy_3,
            /*Params=*/FuncTy_13_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_12 = PointerType::get(FuncTy_13, 0);

    std::vector<Type*>FuncTy_15_args;
    FuncTy_15_args.push_back(IntegerType::get(mod->getContext(), 32));
    FuncTy_15_args.push_back(IntegerType::get(mod->getContext(), 32));
    FuncTy_15_args.push_back(PointerTy_4);
    FunctionType* FuncTy_15 = FunctionType::get(
            /*Result=*/IntegerType::get(mod->getContext(), 32),
            /*Params=*/FuncTy_15_args,
            /*isVarArg=*/false);

    PointerType* PointerTy_14 = PointerType::get(FuncTy_15, 0);

    PointerType* PointerTy_16 = PointerType::get(StructTy_struct_std___Vector_base, 0);

    PointerType* PointerTy_17 = PointerType::get(StructTy_struct_std___Vector_base_PFDSType__std__allocator_PFDSType______Vector_impl, 0);


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

    Function* func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm = mod->getFunction("_ZNSt6vectorI8PFDSTypeSaIS0_EEixEm");
    if (!func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm) {
        func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm = Function::Create(
                /*Type=*/FuncTy_13,
                /*Linkage=*/GlobalValue::LinkOnceODRLinkage ,
                /*Name=*/"_ZNSt6vectorI8PFDSTypeSaIS0_EEixEm", mod); 
        func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm->setCallingConv(CallingConv::C);
        func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm->setAlignment(2);
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
    func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm->setAttributes(func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm_PAL);

    Function* func_add = mod->getFunction("add");
    if (!func_add) {
        func_add = Function::Create(
                /*Type=*/FuncTy_15,
                /*Linkage=*/GlobalValue::ExternalLinkage,
                /*Name=*/"add", mod); // (external, no body)
        func_add->setCallingConv(CallingConv::C);
    }
    AttributeSet func_add_PAL;
    {
        SmallVector<AttributeSet, 4> Attrs;
        AttributeSet PAS;
        {
            AttrBuilder B;
            PAS = AttributeSet::get(mod->getContext(), ~0U, B);
        }

        Attrs.push_back(PAS);
        func_add_PAL = AttributeSet::get(mod->getContext(), Attrs);

    }
    func_add->setAttributes(func_add_PAL);

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
        AllocaInst* ptr_dl_handle_addr = new AllocaInst(PointerTy_4, "dl_handle.addr", label_entry);
        ptr_dl_handle_addr->setAlignment(8);
        AllocaInst* ptr_funcId = new AllocaInst(PointerTy_8, "funcId", label_entry);
        ptr_funcId->setAlignment(8);
        AllocaInst* ptr_paraList = new AllocaInst(PointerTy_2, "paraList", label_entry);
        ptr_paraList->setAlignment(8);
        AllocaInst* ptr_ret = new AllocaInst(PointerTy_3, "ret", label_entry);
        ptr_ret->setAlignment(8);
        AllocaInst* ptr_a = new AllocaInst(IntegerType::get(mod->getContext(), 32), "a", label_entry);
        ptr_a->setAlignment(4);
        AllocaInst* ptr_b = new AllocaInst(IntegerType::get(mod->getContext(), 32), "b", label_entry);
        ptr_b->setAlignment(4);
        StoreInst* void_24 = new StoreInst(ptr_funcT, ptr_funcT_addr, false, label_entry);
        void_24->setAlignment(8);
        StoreInst* void_25 = new StoreInst(ptr_dl_handle, ptr_dl_handle_addr, false, label_entry);
        void_25->setAlignment(8);
        LoadInst* ptr_26 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_26->setAlignment(8);
        std::vector<Value*> ptr_funcId1_indices;
        ptr_funcId1_indices.push_back(const_int32_19);
        ptr_funcId1_indices.push_back(const_int32_19);
        Instruction* ptr_funcId1 = GetElementPtrInst::Create(ptr_26, ptr_funcId1_indices, "funcId1", label_entry);
        StoreInst* void_27 = new StoreInst(ptr_funcId1, ptr_funcId, false, label_entry);
        void_27->setAlignment(8);
        LoadInst* ptr_28 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_28->setAlignment(8);
        std::vector<Value*> ptr_paraList2_indices;
        ptr_paraList2_indices.push_back(const_int32_19);
        ptr_paraList2_indices.push_back(const_int32_18);
        Instruction* ptr_paraList2 = GetElementPtrInst::Create(ptr_28, ptr_paraList2_indices, "paraList2", label_entry);
        LoadInst* ptr_29 = new LoadInst(ptr_paraList2, "", false, label_entry);
        ptr_29->setAlignment(8);
        StoreInst* void_30 = new StoreInst(ptr_29, ptr_paraList, false, label_entry);
        void_30->setAlignment(8);
        LoadInst* ptr_31 = new LoadInst(ptr_funcT_addr, "", false, label_entry);
        ptr_31->setAlignment(8);
        std::vector<Value*> ptr_ret3_indices;
        ptr_ret3_indices.push_back(const_int32_19);
        ptr_ret3_indices.push_back(const_int32_20);
        Instruction* ptr_ret3 = GetElementPtrInst::Create(ptr_31, ptr_ret3_indices, "ret3", label_entry);
        LoadInst* ptr_32 = new LoadInst(ptr_ret3, "", false, label_entry);
        ptr_32->setAlignment(8);
        StoreInst* void_33 = new StoreInst(ptr_32, ptr_ret, false, label_entry);
        void_33->setAlignment(8);
        LoadInst* ptr_34 = new LoadInst(ptr_paraList, "", false, label_entry);
        ptr_34->setAlignment(8);
        std::vector<Value*> ptr_call_params;
        ptr_call_params.push_back(ptr_34);
        ptr_call_params.push_back(const_int64_21);
        CallInst* ptr_call = CallInst::Create(func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm, ptr_call_params, "call", label_entry);
        ptr_call->setCallingConv(CallingConv::C);
        ptr_call->setTailCall(false);
        AttributeSet ptr_call_PAL;
        ptr_call->setAttributes(ptr_call_PAL);

        std::vector<Value*> ptr_pfdsValue_indices;
        ptr_pfdsValue_indices.push_back(const_int32_19);
        ptr_pfdsValue_indices.push_back(const_int32_18);
        Instruction* ptr_pfdsValue = GetElementPtrInst::Create(ptr_call, ptr_pfdsValue_indices, "pfdsValue", label_entry);
        LoadInst* ptr_35 = new LoadInst(ptr_pfdsValue, "", false, label_entry);
        ptr_35->setAlignment(8);
        CastInst* ptr_36 = new BitCastInst(ptr_35, PointerTy_11, "", label_entry);
        LoadInst* int32_37 = new LoadInst(ptr_36, "", false, label_entry);
        int32_37->setAlignment(4);
        StoreInst* void_38 = new StoreInst(int32_37, ptr_a, false, label_entry);
        void_38->setAlignment(4);
        LoadInst* ptr_39 = new LoadInst(ptr_paraList, "", false, label_entry);
        ptr_39->setAlignment(8);
        std::vector<Value*> ptr_call4_params;
        ptr_call4_params.push_back(ptr_39);
        ptr_call4_params.push_back(const_int64_22);
        CallInst* ptr_call4 = CallInst::Create(func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm, ptr_call4_params, "call4", label_entry);
        ptr_call4->setCallingConv(CallingConv::C);
        ptr_call4->setTailCall(false);
        AttributeSet ptr_call4_PAL;
        ptr_call4->setAttributes(ptr_call4_PAL);

        std::vector<Value*> ptr_pfdsValue5_indices;
        ptr_pfdsValue5_indices.push_back(const_int32_19);
        ptr_pfdsValue5_indices.push_back(const_int32_18);
        Instruction* ptr_pfdsValue5 = GetElementPtrInst::Create(ptr_call4, ptr_pfdsValue5_indices, "pfdsValue5", label_entry);
        LoadInst* ptr_40 = new LoadInst(ptr_pfdsValue5, "", false, label_entry);
        ptr_40->setAlignment(8);
        CastInst* ptr_41 = new BitCastInst(ptr_40, PointerTy_11, "", label_entry);
        LoadInst* int32_42 = new LoadInst(ptr_41, "", false, label_entry);
        int32_42->setAlignment(4);
        StoreInst* void_43 = new StoreInst(int32_42, ptr_b, false, label_entry);
        void_43->setAlignment(4);
        LoadInst* int32_44 = new LoadInst(ptr_a, "", false, label_entry);
        int32_44->setAlignment(4);
        LoadInst* int32_45 = new LoadInst(ptr_b, "", false, label_entry);
        int32_45->setAlignment(4);
        LoadInst* ptr_46 = new LoadInst(ptr_dl_handle_addr, "", false, label_entry);
        ptr_46->setAlignment(8);
        std::vector<Value*> int32_call6_params;
        int32_call6_params.push_back(int32_44);
        int32_call6_params.push_back(int32_45);
        int32_call6_params.push_back(ptr_46);
        CallInst* int32_call6 = CallInst::Create(func_add, int32_call6_params, "call6", label_entry);
        int32_call6->setCallingConv(CallingConv::C);
        int32_call6->setTailCall(false);
        AttributeSet int32_call6_PAL;
        int32_call6->setAttributes(int32_call6_PAL);

        LoadInst* ptr_47 = new LoadInst(ptr_ret, "", false, label_entry);
        ptr_47->setAlignment(8);
        std::vector<Value*> ptr_pfdsValue7_indices;
        ptr_pfdsValue7_indices.push_back(const_int32_19);
        ptr_pfdsValue7_indices.push_back(const_int32_18);
        Instruction* ptr_pfdsValue7 = GetElementPtrInst::Create(ptr_47, ptr_pfdsValue7_indices, "pfdsValue7", label_entry);
        LoadInst* ptr_48 = new LoadInst(ptr_pfdsValue7, "", false, label_entry);
        ptr_48->setAlignment(8);
        CastInst* ptr_49 = new BitCastInst(ptr_48, PointerTy_11, "", label_entry);
        StoreInst* void_50 = new StoreInst(int32_call6, ptr_49, false, label_entry);
        void_50->setAlignment(4);
        ReturnInst::Create(mod->getContext(), const_int1_23, label_entry);

    }

    // Function: _ZNSt6vectorI8PFDSTypeSaIS0_EEixEm (func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm)
    {
        Function::arg_iterator args = func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm->arg_begin();
        Value* ptr_this = args++;
        ptr_this->setName("this");
        Value* int64___n = args++;
        int64___n->setName("__n");

        BasicBlock* label_entry_52 = BasicBlock::Create(mod->getContext(), "entry",func__ZNSt6vectorI8PFDSTypeSaIS0_EEixEm,0);

        // Block entry (label_entry_52)
        AllocaInst* ptr_this_addr = new AllocaInst(PointerTy_2, "this.addr", label_entry_52);
        ptr_this_addr->setAlignment(8);
        AllocaInst* ptr___n_addr = new AllocaInst(IntegerType::get(mod->getContext(), 64), "__n.addr", label_entry_52);
        ptr___n_addr->setAlignment(8);
        StoreInst* void_53 = new StoreInst(ptr_this, ptr_this_addr, false, label_entry_52);
        void_53->setAlignment(8);
        StoreInst* void_54 = new StoreInst(int64___n, ptr___n_addr, false, label_entry_52);
        void_54->setAlignment(8);
        LoadInst* ptr_this1 = new LoadInst(ptr_this_addr, "this1", false, label_entry_52);
        CastInst* ptr_55 = new BitCastInst(ptr_this1, PointerTy_16, "", label_entry_52);
        std::vector<Value*> ptr__M_impl_indices;
        ptr__M_impl_indices.push_back(const_int32_19);
        ptr__M_impl_indices.push_back(const_int32_19);
        Instruction* ptr__M_impl = GetElementPtrInst::Create(ptr_55, ptr__M_impl_indices, "_M_impl", label_entry_52);
        std::vector<Value*> ptr__M_start_indices;
        ptr__M_start_indices.push_back(const_int32_19);
        ptr__M_start_indices.push_back(const_int32_19);
        Instruction* ptr__M_start = GetElementPtrInst::Create(ptr__M_impl, ptr__M_start_indices, "_M_start", label_entry_52);
        LoadInst* ptr_56 = new LoadInst(ptr__M_start, "", false, label_entry_52);
        ptr_56->setAlignment(8);
        LoadInst* int64_57 = new LoadInst(ptr___n_addr, "", false, label_entry_52);
        int64_57->setAlignment(8);
        GetElementPtrInst* ptr_add_ptr = GetElementPtrInst::Create(ptr_56, int64_57, "add.ptr", label_entry_52);
        ReturnInst::Create(mod->getContext(), ptr_add_ptr, label_entry_52);

    }

    return mod;
}
//Module* makeLLVMModule(Module* mod) {
//    // Module Construction
//    mod->setDataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128");
//    mod->setTargetTriple("x86_64-unknown-linux-gnu");
//
//    // Type Definitions
//    std::vector<Type*>FuncTy_0_args;
//    PointerType* PointerTy_1 = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
//
//    FuncTy_0_args.push_back(PointerTy_1);
//    FuncTy_0_args.push_back(PointerTy_1);
//    FunctionType* FuncTy_0 = FunctionType::get(
//            /*Result=*/IntegerType::get(mod->getContext(), 1),
//            /*Params=*/FuncTy_0_args,
//            /*isVarArg=*/false);
//
//    PointerType* PointerTy_2 = PointerType::get(PointerTy_1, 0);
//
//    // std::vector<Type*>FuncTy_4_args;
//    // FuncTy_4_args.push_back(IntegerType::get(mod->getContext(), 32));
//    // FuncTy_4_args.push_back(IntegerType::get(mod->getContext(), 32));
//    // FuncTy_4_args.push_back(PointerTy_1);
//    // FunctionType* FuncTy_4 = FunctionType::get(
//    //         /*Result=*/IntegerType::get(mod->getContext(), 32),
//    //         /*Params=*/FuncTy_4_args,
//    //         /*isVarArg=*/false);
//
//    // PointerType* PointerTy_3 = PointerType::get(FuncTy_4, 0);
//
//
//    // Function Declarations
//
//    Function* func_invoke = mod->getFunction("invoke");
//    if (!func_invoke) {
//        func_invoke = Function::Create(
//                /*Type=*/FuncTy_0,
//                /*Linkage=*/GlobalValue::ExternalLinkage,
//                /*Name=*/"invoke", mod); 
//        func_invoke->setCallingConv(CallingConv::C);
//    }
//    AttributeSet func_invoke_PAL;
//    {
//        SmallVector<AttributeSet, 4> Attrs;
//        AttributeSet PAS;
//        {
//            AttrBuilder B;
//            B.addAttribute(Attribute::ZExt);
//            PAS = AttributeSet::get(mod->getContext(), 0U, B);
//        }
//
//        Attrs.push_back(PAS);
//        {
//            AttrBuilder B;
//            B.addAttribute(Attribute::UWTable);
//            PAS = AttributeSet::get(mod->getContext(), ~0U, B);
//        }
//
//        Attrs.push_back(PAS);
//        func_invoke_PAL = AttributeSet::get(mod->getContext(), Attrs);
//
//    }
//    func_invoke->setAttributes(func_invoke_PAL);
//
//    // if (!func_add) {
//    //     std::cout << "add is empty!" << std::endl;
//    //     func_add = Function::Create(
//    //             /*Type=*/FuncTy_4,
//    //             /*Linkage=*/GlobalValue::ExternalLinkage,
//    //             /*Name=*/"add", mod); // (external, no body)
//    //     func_add->setCallingConv(CallingConv::C);
//    // }
//    // AttributeSet func_add_PAL;
//    // {
//    //     SmallVector<AttributeSet, 4> Attrs;
//    //     AttributeSet PAS;
//    //     {
//    //         AttrBuilder B;
//    //         PAS = AttributeSet::get(mod->getContext(), ~0U, B);
//    //     }
//
//    //     Attrs.push_back(PAS);
//    //     func_add_PAL = AttributeSet::get(mod->getContext(), Attrs);
//
//    // }
//    // func_add->setAttributes(func_add_PAL);
//    // func_add->addAttribute(llvm::AttributeSet::FunctionIndex, llvm::Attribute::AlwaysInline);
//
//    // Global Variable Declarations
//
//
//    // Constant Definitions
//    ConstantInt* const_int32_5 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
//    ConstantInt* const_int32_6 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("2"), 10));
//    ConstantInt* const_int1_7 = ConstantInt::get(mod->getContext(), APInt(1, StringRef("-1"), 10));
//
//    // Global Variable Definitions
//
//    // Function Definitions
//
//    // Function: invoke (func_invoke)
//    {
//        Function::arg_iterator args = func_invoke->arg_begin();
//        Value* ptr_funName = args++;
//        ptr_funName->setName("funName");
//        Value* ptr_handle = args++;
//        ptr_handle->setName("handle");
//
//        BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",func_invoke,0);
//
//        // Block entry (label_entry)
//        AllocaInst* ptr_funName_addr = new AllocaInst(PointerTy_1, "funName.addr", label_entry);
//        ptr_funName_addr->setAlignment(8);
//        AllocaInst* ptr_handle_addr = new AllocaInst(PointerTy_1, "handle.addr", label_entry);
//        ptr_handle_addr->setAlignment(8);
//        StoreInst* void_8 = new StoreInst(ptr_funName, ptr_funName_addr, false, label_entry);
//        void_8->setAlignment(8);
//        StoreInst* void_9 = new StoreInst(ptr_handle, ptr_handle_addr, false, label_entry);
//        void_9->setAlignment(8);
//        LoadInst* ptr_10 = new LoadInst(ptr_handle_addr, "", false, label_entry);
//        ptr_10->setAlignment(8);
//        Function* func_add = mod->getFunction("add");
//        std::vector<Value*> int32_call_params;
//        int32_call_params.push_back(const_int32_5);
//        int32_call_params.push_back(const_int32_6);
//        int32_call_params.push_back(ptr_10);
//        CallInst* int32_call = CallInst::Create(func_add, int32_call_params, "call", label_entry);
//        int32_call->setCallingConv(CallingConv::C);
//        int32_call->setTailCall(false);
//        AttributeSet int32_call_PAL;
//        int32_call->setAttributes(int32_call_PAL);
//
//        ReturnInst::Create(mod->getContext(), const_int1_7, label_entry);
//
//    }
//
//    return mod;
//}
