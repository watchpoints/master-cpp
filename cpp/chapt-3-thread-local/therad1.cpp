/*
 * @Date: 2025-05-13 21:09:48
 * @Author: 后端开发成长指南 +v watchpoints
 * @FilePath: 知识地图--高频面试题
 * @Git: https://github.com/watchpoints/master-cpp
 * @blog: 
 * @Video: 
 * @Description:
 */

#include <iostream>
#include <thread>
#include <memory>

//用CPU的视角，解读 thread local 的工作原理
//g++ -S -O2 thread1.cpp -o thread.s
//https://godbolt.org/z/o6njsaan9


thread_local int x = 1;  // 线程局部变量（全局作用域）

void thread_func() {
    std::cout << "Thread " << std::this_thread::get_id() 
              << ": x address = " << &x << std::endl;
              
}

int func()
{
    thread_local int b = 2; // 线程局部变量（函数局部作用域）
    return b;
    // mov   eax, DWORD PTR fs:func()::b@tpoff
}

int main()
{
    std::thread t1(thread_func);   // 创建线程1
    std::thread t2(thread_func);   // 创建线程2
    //同一个变量为什么不同线程，看到不同地址？
    //栈也实现，为什么不栈代替
    // Thread 140289368270400: x address = 0x7f97a9f6263c
    // Thread 140289359877696: x address = 0x7f97a976163c

    t1.join(); 
    t2.join();              

    return 0;
}