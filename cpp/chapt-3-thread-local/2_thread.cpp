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
using namespace std;

//https://compiler-explorer.com/z/PsKMq8PPM

//why 
void thread_func() {
    static thread_local int sum = 0; //like static
    ++sum;
    std::cout << "Thread " << std::this_thread::get_id() 
              << ":count= " << sum << std::endl;
}

int main()
{   
    thread_func();
    thread_func();
    return 0;
}