// demo1.cpp: 定义应用程序的入口点。
//

#include "demo1.h"
#include <iostream>
#include <iomanip> // 包含 std::hex 格式控制器
#include <iostream>
#include <string>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	int hex = 0x1F; // 16进制数
  std::cout << std::dec << hex << std::endl; // 输出10进制数
  std::cout << std::hex << hex << std::endl; // 输出16进制数

	int decimal = std::stoi(hex, 0, 16); // 将16进制数字符串转换为10进制整数
  std::cout << decimal << std::endl; // 输出结果
  
    
	return 0;
}
