#include <iostream>
#include <assert.h>
#include "TestJni.h"
#include "run_link_module.h"
#include "meta_data.h"
void write_ret(JNIEnv* env, jclass retCls, jobject jret, std::string retType, void* retValue) {
    if (retType == "int") {
        jmethodID setVal = env->GetMethodID(retCls, "setValue", "(I)V");
        std::cout << "write_ret: " << *((int*)retValue) << std::endl;
        env->CallVoidMethod(jret, setVal, *((int*)retValue));
    }
}
void write_para(JNIEnv* env, jclass cls_para, jobject obj_para, std::string paraType, std::vector<PFDSType>& pfdsTypeList) {
    if (paraType == "int") {
        jmethodID obj_getValue = env->GetMethodID(cls_para,"getValue","()I");
        jint value = env->CallIntMethod(obj_para, obj_getValue);
        std::cout << "value: " << value << std::endl;
        int* tmp = new int(value);
        PFDSType pfdsType(paraType, (void*)tmp);
        pfdsTypeList.push_back(pfdsType);
    }
}
void* createVoid(std::string type) {
    void* Value = NULL;
    if (type == "int") {
        Value = reinterpret_cast<void*>(new int());
    } else if (type == "int64_t") {
        Value = reinterpret_cast<void*>(new long());
    } else if (type == "double") {
        Value = reinterpret_cast<void*>(new double());
    } else {
        std::cout << "no support" << std::endl;
        return NULL;
    }
    return Value;
}

FuncType* createFuncType(Metadata& me) {
    PFDSType* retPFDSType = new PFDSType(me.m_retType, createVoid(me.retType)); 
    std::vector<PFDSType>* paraListPFDSType = new std::vecor<PFDSType>();
    for(int i = 0; i < me.m_paraTypeList.size(); ++i) {
        PFDSType* paraPFDSType = new PFDSType(me.m_paraTypeList[i].pfdsType, createVoid(me.m_paraTypeList[i].pfdsType)); 
        paraListPFDSType.pust_back(*paraPFDSType);
    }
    FuncType* funcType = new FuncType(me.m_funcName, paraListPFDSType, retPFDSType);
}

JNIEXPORT void JNICALL Java_TestJni_invoke
(JNIEnv *env, jobject obj, jlong jl, jobject jret, jobject jparaList, jobject jstatus) {
    MiniLlvmEngine mle;
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
    std::map<int64_t, MetaElem>::iterator it = mm.find(jl);
    if (it != mm.end()) {
        MetaElem& me = it->second;
        /*
           std::cout << "me.m_retType: " << me.m_retType << std::endl;
           std::cout << "me.m_funcName: " << me.m_funcName << std::endl;
           for (std::vector<std::string>::iterator it = me.m_paraTypeList.begin(); it != me.m_paraTypeList.end(); ++it) {
           std::cout << *it << std::endl;
           }
           */

        FuncType funcType;
        PFDSType retpfdsType;
        funcType.ret = &retpfdsType;
        retpfdsType.pfdsType = me.m_retType;
        int ret = 0;
        retpfdsType.pfdsValue = (void*)&ret;
        std::vector<PFDSType> pfdsTypeList;
        funcType.paraList = &pfdsTypeList;
        funcType.funcName = me.m_funcName;

        // load paraList & len
        jclass cls_arraylist = env->GetObjectClass(jparaList); jmethodID arraylist_get = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");
        jmethodID arraylist_size = env->GetMethodID(cls_arraylist,"size","()I");
        jint len = env->CallIntMethod(jparaList,arraylist_size);

        // assert
        assert(len == (me.m_paraTypeList.size()));

        //std::cout << "get java ArrayList<User> object by C++ , then print it....." << std::endl;
        //std::cout << "len" << len << std::endl;
        for (int i = 0; i < len; i++) {
            jobject obj_para = env->CallObjectMethod(jparaList, arraylist_get, i);
            jclass cls_para = env->GetObjectClass(obj_para);
            write_para(env, cls_para, obj_para, me.m_paraTypeList[i], pfdsTypeList);
        }
        std::vector<PFDSType>* paraList = funcType.paraList;
        //std::cout << "Java_TestJni_invoke: " << *((int*)((*paraList)[0].pfdsValue)) << std::endl;
        //std::cout << "Java_TestJni_invoke: " << *((int*)((*paraList)[1].pfdsValue)) << std::endl;
        JniStatus status; 
        // status.m_hasWrong = true;
        mle.init(funcType);
        mle.execute(funcType, status);


        // write ret
        jclass retCls = env->GetObjectClass(jret);
        write_ret(env, retCls, jret, funcType.ret->pfdsType, funcType.ret->pfdsValue);
        // write status
        jclass statusCls = env->GetObjectClass(jstatus);
        jmethodID setHasWrong = env->GetMethodID(statusCls, "setM_hasWrong", "(Z)V");
        env->CallVoidMethod(jstatus, setHasWrong, status.m_hasWrong?1:0);
        jmethodID setErrorCode = env->GetMethodID(statusCls, "setM_errorCode", "(I)V");
        env->CallVoidMethod(jstatus, setErrorCode, status.m_errorCode);
        jmethodID setErrorMsg = env->GetMethodID(statusCls, "setM_errorMsg", "(Ljava/lang/String;)V");
        env->CallVoidMethod(jstatus, setErrorMsg, env->NewStringUTF(status.m_errorMsg.c_str()));
    } else {
        std::cout << "no find " << jl << " in Metadata" << std::endl;
    }

    //std::cout << "me.m_retType: " << me.m_retType << std::endl;

}

