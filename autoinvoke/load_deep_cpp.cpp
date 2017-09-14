#include "load_deep_cpp.h"
#include "load_deep2_cpp.h"
#include <stdio.h>
extern "C" {
    int deep_add(int a, int b) {
        A aa(a, b);
        return aa.add();
    }

}
