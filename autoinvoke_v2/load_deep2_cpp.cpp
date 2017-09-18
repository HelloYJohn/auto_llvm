#include "load_deep2_cpp.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

int deep2_add(int a, int b) {
    printf("hello llvm bc\n");
    std::cout << "Hello Llvm Bc" << std::endl;
    typedef std::vector<boost::any> many;
    many any;
    any.push_back(2);
    any.push_back(std::string("test"));
    for (unsigned int i = 0; i < any.size(); ++i) {
        std::cout << any[i].type().name() << std::endl;
    }
    std::cout << boost::lexical_cast<int>("123") << std::endl;
    return a + b;
}
/*
A::A() {
    m_par1 = 0;
    m_par2 = 0;
}

A::A(int par1, int par2) {
    m_par1 = par1;
    m_par2 = par2;
}

A::A(const A& a) {
    m_par1 = a.m_par1;
    m_par2 = a.m_par2;
}

A& A::operator = (const A& a) {
   m_par1 = a.m_par1;
   m_par2 = a.m_par2;
}
*/
int A::add() {
    return m_par1 + m_par2;
}


int A::add(int arg) {
    return m_par1 + m_par2 + arg;
}

