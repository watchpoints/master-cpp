# 设置cmake的最低版本
cmake_minimum_required (VERSION 3.20)
# 设置工程名称 和版本
project ("Function")
# set the C++ standard to C++ 11
set(CMAKE_CXX_STANDARD 20)

# 将源代码添加到此项目的可执行文件。
add_executable (test "ep01_string_view.cpp")

# TODO: 如有需要，请添加测试并安装目标。
