# 设置cmake的最低版本
cmake_minimum_required (VERSION 3.20)
# 设置工程名称 和版本
project ("Function")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0")


set(CMAKE_CXX_STANDARD 20)

# 将代码文件编译成 可执行程序 
add_executable (task04 "day4-cpp11.cpp")
