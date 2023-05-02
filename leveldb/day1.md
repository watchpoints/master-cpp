


践行do-book:精通LevelDB（一）:源码编译安装

### Building
- 作者 https://github.com/erpeng

git clone --recurse-submodules https://github.com/watchpoints/leveldb.git

git submodule update --init

Build for POSIX
Quick start:

mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .



