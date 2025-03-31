#include <iostream>
#include <cstdlib>
#include <string>

/**
1  new运算符和operator new()  区别于联系
void* ptr = operator new(sizeof(int));  // 只分配内存，不调用构造函数
int* p = new int(42);  // 分配内存 + 调用构造函数

如何判断 operator new 是函数，而 new 是运算符？

https://en.cppreference.com/w/cpp/language/new
2 
由于 new 关键字属于 C++ 语言的 语法结构，它不能像其他运算符（如 +、-）一样被重载


*/


// 重载 new 运算符
void* operator new(size_t size) {
    std::cout << ">>>>>>> Custom operator new: allocating " << size << " bytes\n";
    void* ptr = std::malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

// 重载 delete 运算符
void operator delete(void* ptr) noexcept {
    std::cout << ">>>>>>>> Custom operator delete: freeing memory\n";
    std::free(ptr);
}

class MyClass {
public:
    MyClass() { std::cout << "Constructor\n"; }
    ~MyClass() { std::cout << "Destructor\n"; }

     //你可以在自己的类中重载 operator new 和 operator delete，从而定制对象的内存分配方式。
    static void* operator new(size_t size) {
        std::cout << "Custom new: Allocating " << size << " bytes\n"; //1
        return malloc(size);  // 只分配内存
    }

    static void operator delete(void* ptr) {
        std::cout << "Custom delete: Freeing memory\n";
        free(ptr);  // 释放内存
    }
  
};

 
struct MyMalloc{
    int x;
    MyMalloc() { std::cout << "MyMalloc Constructor called\n"; }
};

void test_malloc() {
    //malloc(size_t size) 是 C/C++ 中的标准库函数
    MyMalloc* ptr = (MyMalloc*)malloc(sizeof(MyMalloc));
    //init
    if (ptr) {
        ptr->x = 10;
    }
}

/** 
void new() {  // 语法错误，不能重载 `new`
    std::cout << "Custom new operator\n";
}**/

void test_new() {
 //void* p = new (1024);

  string *ptr =new string("Memory Management");

  Foo* foo = new Foo();
}

class Foo
{
public:
        void* operator new( size_t )
        {
            std::cout << "Custom new: Allocating " << size << " bytes\n"; //1
            return malloc(size);  // 只分配内存
        }
}

int main() {
    test_new();
    MyClass* obj = new MyClass();  // 使用自定义的 new
    delete obj;  // 使用自定义的 delete
    return 0;
}



//new 运算符是 C++ 提供的高级封装，它的工作流程如下：

//调用 operator new 来分配内存。

//调用构造函数 在分配的内存上创建对象。

/** 
void* memory = operator new(sizeof(MyClass));  // 1. 仅分配内存
MyClass* obj = static_cast<MyClass*>(memory);
new (obj) MyClass();  // 2. 在分配的内存上调用构造函数（placement new）

obj->~MyClass();  // 3. 显式调用析构函数
operator delete(obj);  // 4. 释放内存
**/


//VS中去掉代码编辑器中的鼠标光标阴影-- 点击Fn键+insert键即可解决