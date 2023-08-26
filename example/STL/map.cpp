#include <iostream>
#include <map>

int main() {
    std::map<int, std::string> myMap; 
		//Q1为什么指定<>类型，预处理阶,编译阶段,汇编，链接 那个阶段完成的？

		//Q2：编译器（Compiler）和链接器（Linker）完成
		//编译器负责处理单个源文件，将其转换为目标文件 GCC Clang Microsoft Visual C++
		//链接器用于将多个目标文件组合在一起，创建最终的可执行程序或库文
		//GNU链接器（GNU Linker）：也称为ld，icrosoft的链接器，也称为link.exe
    
    myMap[1] = "Apple";
    myMap[2] = "Banana";
    myMap[3] = "Orange";
    myMap[4] = "Grape";

    //Q3 请写一个map 删除代码例子
    //要点：
    //1 map<int, std::string>::iterator it  不拼拼写吗
    //Map.erase  记得清楚吗？
    //++it不在循环里面有，习惯吗
    for (auto it = myMap.begin(); it != myMap.end(); ) {
        if (it->first == 2) {
            // 删除指定记录
            it = myMap.erase(it);//
        } else {
            ++it;
        }
    }
		
    // 输出删除记录后的 map
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
//https://en.cppreference.com/w/cpp/container/vector/erase