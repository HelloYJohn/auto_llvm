#include <iostream>
#include "meta_data.h"
using namespace std;
/*
int main() {
    std::map<int64_t, MetaElem>& mm = createMetaMap();
    for (std::map<int64_t, MetaElem>::iterator it = mm.begin(); it != mm.end(); ++it) {
        std::cout << "it->first: " << it->first << std::endl;
        std::cout << "it->second: " << std::endl;
        std::cout << it->second.m_funcName << std::endl;
        for (std::vector<std::string>::iterator pit = it->second.m_paraTypeList.begin(); pit != it->second.m_paraTypeList.end(); ++ pit) {
            std::cout << *pit << std::endl;
        }
        std::cout << it->second.m_retType << std::endl;
    }
    return 0;
}
*/

std::map<int64_t, MetaElem>& createMetaMap() {
   static std::map<int64_t, MetaElem> MetaMap; 
   std::vector<std::string> ptl;
   ptl.push_back("int");
   ptl.push_back("int");
   ptl.push_back("void*");
   MetaElem me(std::string("add"), ptl, std::string("int"));
   int64_t funcId = __FUNC_ID(1, 1);
   MetaMap.insert(std::make_pair(funcId,me)); 
   return MetaMap;
}
