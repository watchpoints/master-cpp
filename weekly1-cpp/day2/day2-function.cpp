#include <iostream>
using namespace std;


class Base {
    public:
        void func(int x) { }         // 非虚函数，版本1
        virtual void func(double d) { } // 虚函数，版本2
    };
    
class Derived : public Base {
public:
    // 情况1：参数不同（const char*），隐藏了Base中所有的func函数
    void func(const char* s) { } 
};

void test1()
{
    Derived d;
    d.func("Hello"); // 正确：调用Derived::func(const char*)
    // d.func(10);    // 错误！Base::func(int) 已被隐藏
    // d.func(3.14);  // 错误！Base::func(double) 也被隐藏了

    // 必须使用作用域解析运算符来显式调用基类被隐藏的函数
    d.Base::func(10);  // 正确：调用Base::func(int)
    d.Base::func(3.14); // 正确：调用Base::func(double)
}
int main() {
    test1();
    return 0;
}

