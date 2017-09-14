#include "HelloJni.h"  
#include <iostream>
#include <sstream>
int i=1;  
jobject user;  
JNIEXPORT jint JNICALL Java_HelloJni_get  
(JNIEnv *env, jobject jthiz){  
    printf("HelloWorld\n");  
}  
JNIEXPORT void JNICALL Java_HelloJni_setUser__Ljava_lang_String_2
(JNIEnv *env, jobject jthiz, jstring name){  
    jclass userClass = env->FindClass("User");  
    jmethodID userMethod = env->GetMethodID(userClass,"<init>","()V");  
    jfieldID mId = env->GetFieldID(userClass,"id","J");  
    jfieldID mUserName = env->GetFieldID(userClass,"userName","Ljava/lang/String;");  
    jfieldID mIsMan = env->GetFieldID(userClass,"isMan","Z");  
    jfieldID mAge = env->GetFieldID(userClass,"age","I");  
    jobject userObject = env->NewObject(userClass,userMethod);  
    env->SetObjectField(userObject,mUserName,name);  
    env->SetLongField(userObject,mId,101);  
    env->SetBooleanField(userObject,mIsMan,1);  
    env->SetIntField(userObject,mAge,24);  
    user = userObject;  
}  


JNIEXPORT void JNICALL Java_HelloJni_getUser
(JNIEnv * env, jobject jthiz, jobject userObject) {
    jclass userClass = env->FindClass("User");
    jfieldID mId = env->GetFieldID(userClass,"id","J");
    jfieldID mUserName = env->GetFieldID(userClass,"userName","Ljava/lang/String;");
    jfieldID mIsMan = env->GetFieldID(userClass,"isMan","Z");
    jfieldID mAge = env->GetFieldID(userClass,"age","I");
    env->SetObjectField(userObject,mUserName,env->NewStringUTF("cLiangYaoTian"));
    env->SetLongField(userObject,mId,1001);
    env->SetBooleanField(userObject,mIsMan,1);
    env->SetIntField(userObject,mAge,24);
}

JNIEXPORT void JNICALL Java_HelloJni_setUserList  
(JNIEnv *env, jobject jthiz, jobject userList){  
    int i;  
    //class ArrayList  
    jclass cls_arraylist = env->GetObjectClass(userList);  
    //method in class ArrayList  
    jmethodID arraylist_get = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");  
    jmethodID arraylist_size = env->GetMethodID(cls_arraylist,"size","()I");  
    jint len = env->CallIntMethod(userList,arraylist_size);  
    printf("get java ArrayList<User> object by C++ , then print it.....\n");  
    for(i=0;i<len;i++){  
        jobject obj_user = env->CallObjectMethod(userList,arraylist_get,i);  
        jclass cls_user = env->GetObjectClass(obj_user);  
        jmethodID user_getId = env->GetMethodID(cls_user,"getId","()J");  
        jmethodID user_getUserName = env->GetMethodID(cls_user,"getUserName","()Ljava/lang/String;");  
        jmethodID user_isMan = env->GetMethodID(cls_user,"isMan","()Z");  
        jmethodID user_getAge = env->GetMethodID(cls_user,"getAge","()I");  


        jstring name = (jstring)env->CallObjectMethod(obj_user,user_getUserName);  
        jboolean b = 1;  
        const char *namePtr = env->GetStringUTFChars(name,&b);  
        jlong id = env->CallLongMethod(obj_user,user_getId);  
        jboolean sex = env->CallBooleanMethod(obj_user,user_isMan);  
        jint age = env->CallIntMethod(obj_user,user_getAge);  
        printf("Id:%d; ",id);  
        printf("Name:%s; ",namePtr);  
        printf("isMan %d; ",sex);  
        printf("Age:%d \n",age); 

        //get field id  
        /*
           jfieldID user_id = env->GetFieldID(cls_user,"id","J");  
           jfieldID user_name = env->GetFieldID(cls_user,"userName","Ljava/lang/String;");  
           jfieldID username_isMan = env->GetFieldID(cls_user,"isMan","Z");  
           jfieldID user_age = env->GetFieldID(cls_user,"age","I");  
           env->SetLongField(obj_user,user_id,i);  
           std::stringstream ss;
           ss << "john_" << i;
           env->SetObjectField(obj_user,user_name,env->NewStringUTF(ss.str().c_str()));  
           env->SetBooleanField(obj_user,username_isMan,1);  
           env->SetIntField(obj_user,user_age,24 + i);  
           */
        jmethodID user_setId = env->GetMethodID(cls_user,"setId","(J)V"); 
        jmethodID user_setUserName = env->GetMethodID(cls_user,"setUserName","(Ljava/lang/String;)V"); 
        jmethodID user_setMan = env->GetMethodID(cls_user,"setMan","(Z)V"); 
        jmethodID user_setAge = env->GetMethodID(cls_user,"setAge","(I)V"); 

        env->CallVoidMethod(obj_user,user_setId, i); 
        std::stringstream ss;
        ss << "John_" << i;
        env->CallVoidMethod(obj_user,user_setUserName, env->NewStringUTF(ss.str().c_str()));
        env->CallVoidMethod(obj_user,user_setMan, 1); 
        env->CallVoidMethod(obj_user,user_setAge, 24 + i);
    }
}  
JNIEXPORT void JNICALL Java_HelloJni_set  
(JNIEnv *jenv, jobject obj, jobject data) {
/*
    jclass peerCls = jenv->GetObjectClass(data);

    jmethodID mGetValue = jenv->GetMethodID(peerCls, "getValue","()Ljava/lang/Integer;");
    if (mGetValue == NULL){
        std::cout << "mGetValue is NULL" << std::endl;
        return;
    }

    jobject value = jenv->CallObjectMethod(data, mGetValue);
    if(value == NULL){
        std::cout<<"jobject value = NULL"<<std::endl;
        return;
    }

    //getValue()

    jclass cls = jenv->FindClass("java/lang/Integer");
    if(cls == NULL){
        std::cout <<"cannot find FindClass(java/lang/Integer)"<<std::endl;
        return;
    }

    jmethodID getVal = jenv->GetMethodID(cls, "intValue", "()I");
    if(getVal == NULL){
        std::cout <<"Couldnot find Int getValue()"<< std::endl;
        return;
    }
    int i = jenv->CallIntMethod(value, getVal);
    std::cout << i << std::endl;
    */
    jclass peerCls = jenv->GetObjectClass(data);
    jmethodID getVal = jenv->GetMethodID(peerCls, "intValue", "()I");
    jint value = jenv->CallIntMethod(data, getVal);
    std::cout << value << std::endl;
}

JNIEXPORT jobject JNICALL Java_HelloJni_getUserList  
(JNIEnv *env, jobject jthiz){  
    //ArrayList Object  
    jclass cls_ArrayList = env->FindClass("java/util/ArrayList");  
    jmethodID construct = env->GetMethodID(cls_ArrayList,"<init>","()V");  
    jobject obj_ArrayList = env->NewObject(cls_ArrayList,construct,"");  
    jmethodID arrayList_add = env->GetMethodID(cls_ArrayList,"add","(Ljava/lang/Object;)Z");  
    //User Object  
    jclass cls_user = env->FindClass("User");  
    //none argument construct function  
    jmethodID construct_user = env->GetMethodID(cls_user,"<init>","()V");  
    //new a object  
    jobject obj_user = env->NewObject(cls_user,construct_user,"");  
    //get method id  
    /* 
       jmethodID user_setId = env->GetMethodID(cls_user,"setId","(J)V"); 
       jmethodID user_setUserName = env->GetMethodID(cls_user,"setUserName","(Ljava/lang/String;)V"); 
       jmethodID user_setMan = env->GetMethodID(cls_user,"setMan","(Z)V"); 
       jmethodID user_setAge = env->GetMethodID(cls_user,"setAge","(I)V"); 
       */  
    int i;  
    for(i=0;i<10;i++){  
        //new a object  
        jobject obj_user = env->NewObject(cls_user,construct_user,"");  
        //get field id  
        jfieldID user_id = env->GetFieldID(cls_user,"id","J");  
        jfieldID user_name = env->GetFieldID(cls_user,"userName","Ljava/lang/String;");  
        jfieldID user_isMan = env->GetFieldID(cls_user,"isMan","Z");  
        jfieldID user_age = env->GetFieldID(cls_user,"age","I");  
        env->SetLongField(obj_user,user_id,i);  
        env->SetObjectField(obj_user,user_name,env->NewStringUTF("cLiangYaoTian"));  
        env->SetBooleanField(obj_user,user_isMan,1);  
        env->SetIntField(obj_user,user_age,21);  
        env->CallObjectMethod(obj_ArrayList,arrayList_add,obj_user);  
    }  
    return obj_ArrayList;  
} 
