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

# 用法：CMAKE_CXX_STANDARD set the C++ standard to 20版本
# 隐藏知识点：gcc版本与c++标准库的关系（关系是啥，现在模糊的）
# c++17 gcc版本8.4  比较成熟，至少7.X
# c++20  从GCC 8版本开始陆续支持C++20特性	
# 判断题：别人通过提问判断有没有说谎，如果回答 采用c++20开发，gcc版本7.4，说谎呀，基本常识没有
# 反应速度：如果犹豫表情痛苦回答，说明 这一块根本研究过，经过回忆 出来也不行。
# 1分钟快问快答，回答简短 准确才是超预期，不然判断说谎，不牢固直接pass
# 请问你采用c++那个标准库 ?C++17，为不是c++20 互联网企业 采用最版本，传统企业保守不是最新版本
# 请问gcc版本多少？gcc版本8.4
# 简单问答 看透一人底裤

set(CMAKE_CXX_STANDARD 20)

# 将代码文件编译成 可执行程序 
add_executable (hello "my_dlopen.cpp")
