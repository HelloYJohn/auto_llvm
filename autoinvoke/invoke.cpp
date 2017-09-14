#include <string>
#include <vector>
#include <stdio.h>
#include <stdint.h>
extern "C" {
    int add(int a , int b, void* dl_handle);
}
struct PFDSType {
    std::string pfdsType;
    void* pfdsValue;
    PFDSType() {
        pfdsType = "NULL";
        pfdsValue = NULL;
    }
    PFDSType(std::string pfdsT, void* pfdsV) {
        pfdsType = pfdsT;
        pfdsValue = pfdsV;
    }
};

struct FuncType {
    int64_t funcId;
    std::vector<PFDSType>* paraList;
    PFDSType *ret;
    FuncType():funcId(0), paraList(NULL), ret(NULL) {
    }
    FuncType(int64_t fid, std::vector<PFDSType>* pl, PFDSType* r):funcId(fid), paraList(pl), ret(r) {
    }
};
/*
   bool invoke(FuncType funcT, void* dl_handle) {
   int64_t& funcId = funcT.funcId;
   std::vector<PFDSType>& paraList = funcT.paraList;
   PFDSType& ret = funcT.ret;
   int a = *((int*)(paraList[0].pfdsValue));
   int b = *((int*)(paraList[1].pfdsValue));
   int *r = new int(add(a , b, dl_handle));
   ret.pfdsValue = (void*)r;
   return true;
   }
   */
extern "C" {
    bool invoke(FuncType& funcT, void* dl_handle) {
        int64_t& funcId = funcT.funcId;
        std::vector<PFDSType>* paraList = funcT.paraList;
        PFDSType* ret = funcT.ret;
        int a = *((int*)((*paraList)[0].pfdsValue));
        int b = *((int*)((*paraList)[1].pfdsValue));
        (*(int*)(ret->pfdsValue)) = add(a , b, dl_handle);
        return true;
    }
}
