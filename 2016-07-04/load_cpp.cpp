#include "load_deep_cpp.h"
#include <dlfcn.h>

extern "C" {
/*
void func(void* arg1, void* arg2, void* res) {
     
}
*/
int add(int a , int b)
{
    return deep_add(a, b);

}

int sub(int a , int b)
{
	return deep_sub(a, b);
}

}
