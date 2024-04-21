
#include <iostream>
#include <iomanip> // 包含 std::hex 格式控制器
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
//g++ -g -Wall -std=c++11 main.cpp
int main()
{
	cout << "Hello CMake." << endl;
    //?? int x = 0x1F; 语句在C++中是合法的
	//十六进制字面值初始化整数变量的常见方式。
	// 在这个例子中，0x1F 是一个十六进制数，等于十进制的31。因此，整数变量 x 被赋值为31。
	 int hex = 0x1F; // 在C++中，大多数字面值是右值（R-value）
	//int hex = 1F; // 16进制数
	//0x 是一个指示十六进制数的前缀
	std::cout << std::dec << hex << std::endl; // 输出10进制数
	std::cout << std::hex << hex << std::endl; // 输出16进制数


	// 无符号64位整数的十六进制字面值
	//这是因为在C++中，默认将没有前缀的整数字面值解释为十进制
    // uint64_t hexValue = FEDCBA9876543210;
	// std::cout << "Hex Value: " << std::hex << ":" << hexValue << std::endl;
	// std::cout << "Hex Value: " << std::dec << ":" << hexValue << std::endl;

	stringstream ss2;
    int d2;
    string str2("1aF"); //1aF十进制431
    ss2 << std::hex << str2; //选用十六进制输出

    ss2 >> d2;
    std::cout << "1aF="<< d2 << std::endl;
    std::cout <<"----------------------"<<std::endl;;

	uint64_t aa = 656111;
	std::cout << std::hex <<  "0x" << aa << std::dec << std::endl; // 输出16进制数


    
	return 0;
}
