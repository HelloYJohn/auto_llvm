#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#define __FUNC_ID(x, y) ((((x) & 0xFFFFFFFFFFFFFFFF) << 32) | ((y) & 0xFFFFFFFFFFFFFFFF))
struct MetaElem {
    std::string m_funcName;
    std::vector<std::string> m_paraTypeList;
    std::string m_retType;
    MetaElem(std::string funcName, std::vector<std::string>& paraTypeList, std::string ret_type) {
        m_funcName = funcName;
        m_paraTypeList = paraTypeList;
        m_retType = ret_type;
    }
    MetaElem() {
    }
};
std::map<int64_t, MetaElem>& createMetaMap();
