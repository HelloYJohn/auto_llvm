#include <dlfcn.h>
#include <stdio.h>
#include "load_cpp.h"
#include "load_deep_cpp.h"
extern "C" {
    /*
       void func(void* arg1, void* arg2, void* res) {

       }
       */

    typedef int(*deep_add_func)(int, int);
    int add(int a , int b, void* dl_handle)
    {
        // deep_add_func deep_add = reinterpret_cast<deep_add_func>(dlsym(dl_handle, "deep_add"));
        deep_add_func deep_add = (deep_add_func)(dlsym(dl_handle, "deep_add"));
        char *error = dlerror();
        if (error != NULL) {
            printf( "!!! %s\n", error );
            return 0;
        }
        printf("a = %d\n", a);
        printf("b = %d\n", b);
        printf("deep_add = %d\n", deep_add(a, b));
        return deep_add(a, b);
    }


}
