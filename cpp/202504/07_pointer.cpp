#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int &b = a;

    cout << "Address of a: " << &a << endl;
    cout << "Address of b: " << &b << endl;
    //int * ptr = move(a); //error: invalid conversion from ‘std::remove_reference<int&>::type’
   // int && ptr = move(&a); //error: invalid conversion from ‘std::remove_reference<int&>::type’
   //int* ptr3  = 10; //invalid conversion from ‘int’ to ‘int*’
   int* ptr3  = new int(10);

    //引用不会分配自己的内存地址，它会直接绑定到原变量的地址上。

    //为什么 std::move 不“移动”成指针

    return 0;
}