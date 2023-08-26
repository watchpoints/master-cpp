#include <iostream>
#include <type_traits>
using namespace std;

void f(int &&a) {
    std::cout << "rvalue" << std::endl;
}

void f(int &a) {
    std::cout << "lvalue" << std::endl;
}
void test1()
{
  f(a);
}
// template<typename T>
// void f(T&& a) {
//     if constexpr(is_lvalue<decltype((a))>){
//         std::cout << "lvalue" << std::endl;
//     }else if constexpr(is_xvalue<decltype((a))>){
//         std::cout << "xvalue" << std::endl;
//     }
// }

int add(int a, int b, int c)
{
	return a + b + c;
}

