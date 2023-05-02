


践行do-book:精通LevelDB（一）:源码编译安装

1. Building

- 作者 https://github.com/erpeng
~~~
git clone --recurse-submodules https://github.com/watchpoints/leveldb.git

git submodule update --init

Build for POSIX
Quick start:

mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .~~

make install # 安装头文件
~~~

2. 编译自己的CMakeLists.txt

g++ -o ken_test ken_test.cpp -lleveldb -lpthread -I../leveldb-master/include/ -L../leveldb-master/build/ -std=c++11









