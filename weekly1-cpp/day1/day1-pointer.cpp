#include <iostream>
#include "day1.h"
using namespace std;

void bar(int && a)
{
    cout << " a is right value" << endl;
}

void bar(int & a)
{
    cout << " a is left value" << endl;
}

void test1()
{
    //有名字的变量（无论是 T、T&、T&&）一律是左值。
    int&& r = 10; // r 绑定右值
    bar(r);       // r 是左值
    bar(std::move(r)); // ✅ 恢复右值特性
}

void test_pointer()
{
 
    int a = 10;
    int *p = &a;  // 明确的取地址
    *p = 20;      // 明确的解引用

    // 如果C有引用，会隐藏底层操作
    // int &ref = a;  // 引用看起来像普通变量，但行为不同
    // ref = 20;      // 实际修改了a，但语法上看不出来

    Node* ptr = new Node();

    delete ptr;
}
void test_class()
{
    A* a = new A;  // 输出: A::operator new, size = 1
    delete a;       // 输出: A::operator delete
}
int main()
{
    int rc = 0;
    cout << "task01" << endl;
    test1();
    test_class();
    return rc;
}
