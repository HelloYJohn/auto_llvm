#include "load_deep_cpp.h"
#include "load_deep2_cpp.h"

int deep_add(int a, int b) {
    A aa(a, b);
    return aa.add(3) + aa.add();
}
/*

int deep_sub(int a, int b) {
    return a - b;
}
*/
