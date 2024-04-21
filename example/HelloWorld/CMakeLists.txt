# CMakeList.txt: demo1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
cmake_minimum_required (VERSION 3.8)

project ("HelloWorld")

# 将源代码添加到此项目的可执行文件。
# 通过指定的源文件列表构建出可执行目标文件
add_executable (test "main.cpp")
