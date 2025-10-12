# 设置cmake的最低版本
cmake_minimum_required (VERSION 3.20)
# 设置工程名称 和版本
project ("Function")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0")
#用法：Set a default C++ compile flag
# 隐藏知识点：如何判断使用gcc编译器 还是clang编译器看c++标准库的关系？
# libstdc++ 是 gcc 编译器系统中的标准 C++ 库
# libc++ 是 clang 编译器特别重写的标准 C++ 库
# libc.so 是 Linux 下最早支持的标准 C 库
# 隐藏知识点：-O3 相差 15 倍以上。  https://www.baihezi.com/post/222289.html
# gcc 和 g++ 都是根据 文件的后缀名称 .c .cpp；来判断是否以 C语言 还是 c++ 来编译文件。且g++会自动链接标准库STL



set(CMAKE_CXX_STANDARD 20)

# 将代码文件编译成 可执行程序 
add_executable (task01 "day1-pointer.cpp")
