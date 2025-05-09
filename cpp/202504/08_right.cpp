#include <iostream>
#include <utility>  // 需要包含此头文件才能用std::forward

void process(int& ptr) {
    std::cout << "左值处理: " << ptr << "\n";
}

void process(int&& ptr) {
    std::cout << "右值处理: " << ptr << "\n";
}

template <typename T>
void wrapper1(T&& arg) {  // 万能引用参数
    process(std::forward<T>(arg));  // 正确转发值类别
}

template <typename T>
void wrapper(T&& arg) {  // 万能引用参数
    process(std::forward<T*>(arg));  // 正确转发值类别
}
int main() {
    int x = 10;
    wrapper(x);        // 左值调用：输出"左值处理: 10"
    wrapper(20);       // 右值调用：输出"右值处理: 20"
    wrapper(std::move(x)); // 显式转为右值：输出"右值处理: 10"
}