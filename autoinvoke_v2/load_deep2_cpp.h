int deep2_add(int a, int b);
class A {

private:
    int m_par1, m_par2;
public:
    A() {
        m_par1 = 0;
        m_par2 = 0;
    }
    
    A(int par1, int par2) {
        m_par1 = par1;
        m_par2 = par2;
    }
    

    A(const A& a) {
        m_par1 = a.m_par1;
        m_par2 = a.m_par2;
    }

    A& operator = (const A& a) {
        if (this == &a) {
            return *this;
        } else {
            m_par1 = a.m_par1;
            m_par2 = a.m_par2;
            return *this;
        }
    }

    int add();
    int add(int arg);
    ~A() {
    }
};
/*
class A {

private:
    int m_par1, m_par2;
public:
    A();
    A(int par1, int par2);
    A(const A& a);
    A& operator = (const A& a);
    int add();
    ~A();
};

*/
