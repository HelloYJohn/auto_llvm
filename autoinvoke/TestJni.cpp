#include <iostream>
#include <assert.h>
#include "TestJni.h"
#include "run_link_module.h"
#include "meta_data.h"

JNIEXPORT void JNICALL Java_TestJni_invoke
(JNIEnv *env, jobject obj, jlong jl, jobject jret, jobject jparList) {
    MiniLlvmEngine mle;
    mle.init();
    std::map<int64_t, MetaElem>& mm = createMetaMap();
    /*
    for (std::map<int64_t, MetaElem>::iterator it = mm.begin(); it != mm.end(); ++it) {
        std::cout << "it->first: " << it->first << std::endl;
        std::cout << "it->second: " << std::endl;
        std::cout << it->second.m_funcName << std::endl;
        for (std::vector<std::string>::iterator pit = it->second.m_paraTypeList.begin(); pit != it->second.m_paraTypeList.end(); ++ pit) {
            std::cout << *pit << std::endl;
        }
        std::cout << it->second.m_retType << std::endl;
    } */
    MetaElem& me = mm[jl];
    //std::cout << "me.m_retType: " << me.m_retType << std::endl;
    FuncType funcType;
    funcType.funcId = jl;
    PFDSType retpfdsType;
    funcType.ret = &retpfdsType;
    retpfdsType.pfdsType = me.m_retType;
    int ret = 0;
    retpfdsType.pfdsValue = (void*)&ret;
    std::vector<PFDSType> pfdsTypeList;
    funcType.paraList = &pfdsTypeList;

    // load para
    jclass cls_arraylist = env->GetObjectClass(jparList);
    jmethodID arraylist_get = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");
    jmethodID arraylist_size = env->GetMethodID(cls_arraylist,"size","()I");
    jint len = env->CallIntMethod(jparList,arraylist_size);

    // assert
    assert(len == (me.m_paraTypeList.size() - 1));

    //std::cout << "get java ArrayList<User> object by C++ , then print it....." << std::endl;
    //std::cout << "len" << len << std::endl;
    for (int i = 0; i < len; i++) {
        jobject obj_para = env->CallObjectMethod(jparList, arraylist_get, i);
        jclass cls_para = env->GetObjectClass(obj_para);
        jmethodID obj_getValue = env->GetMethodID(cls_para,"getValue","()I");
        jint value = env->CallIntMethod(obj_para, obj_getValue);
        // std::cout << "value: " << value << std::endl;
        int* tmp = new int(value);
        PFDSType pfdsType(me.m_paraTypeList[i], (void*)tmp);
        pfdsTypeList.push_back(pfdsType);
    }
    std::vector<PFDSType>* paraList = funcType.paraList;
    //std::cout << "Java_TestJni_invoke: " << *((int*)((*paraList)[0].pfdsValue)) << std::endl;
    //std::cout << "Java_TestJni_invoke: " << *((int*)((*paraList)[1].pfdsValue)) << std::endl;


    mle.execute(funcType);


    // write ret
    jclass peerCls = env->GetObjectClass(jret);
    jmethodID setVal = env->GetMethodID(peerCls, "setValue", "(I)V");
    env->CallVoidMethod(jret, setVal, *((int*)funcType.ret->pfdsValue));

}

