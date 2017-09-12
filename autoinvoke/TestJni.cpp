#include <iostream>
#include "TestJni.h"
#include "run_link_module.h"

JNIEXPORT void JNICALL Java_TestJni_invoke
  (JNIEnv *env, jobject obj, jstring jstr) {
    const char* str = env->GetStringUTFChars(jstr, JNI_FALSE);
    load_bc(str); 
    env->ReleaseStringUTFChars(jstr, str);
}
