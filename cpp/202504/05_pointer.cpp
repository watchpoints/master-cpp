#include <cstring>
#include <memory>
#include <iostream>

using namespace std;

void test_move_function()
{
    std::unique_ptr<int> p1 = std::make_unique<int>(42);
    //这个情况下不能传递指针。
    //std::unique_ptr<int> p2(p1.get());  // ❌ 这样“能不能传指针给构造函数？
    // p1.get() 返回的是裸指针（int*），并没有释放 p1 的所有权
    // 所以 p2 拿到指针以后，以为“自己是主人”，释放时就会 delete
    // free(): double free detected in tcache 2
    // Aborted

    std::unique_ptr<int> p3(std::move(p1));  // ✅ OK，资源所有权转移
}

void test_pointer() {
   //  int &a = 10; 
    const int &a = 10; // cannot bind rvalue
    //const int & ptr1 = new int(20);  //invalid conversion from ‘int*’ to ‘int’
    int * ptr2 = new int(10);
    //int &ptr = ptr2;
   // delete ptr; // type ‘int’ argument given to ‘delete’, expected pointer
   delete ptr2;
}

void test_memcyp() {
    const char source[] = "Hello, memcpy!";
    char destination[20];             // 确保足够大
    // 复制 source 整个数组（包括末尾的 '\0'）
    memcpy(destination, source, sizeof(source));
    int a =10;
    int b =20;
    memcpy(&a,&b,sizeof(int));

    std::cout << destination << "\n"; // 输出：Hello, memcpy!

    // auto_ptr< string> ps (new string ("I reigned lonely as a cloud");
    // auto_ptr<string> vocation; 
    // vocaticn = ps; //转义了。
}

void test_3() {
    unique_ptr<int> only_ptr(new int(10));
    unique_ptr<int> ptr = std::move(only_ptr);
}

void RFn(int&& arg){
 cout << "====&&====";
}

template<typename T>
void ProxyFn(T&& arg){
      //RFn(arg); //cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’ 
      RFn(std::forward<T>(arg));
}

int main() {
    ProxyFn(1);
    // test_3();
    //test_move_function();
   // test_pointer();
 }