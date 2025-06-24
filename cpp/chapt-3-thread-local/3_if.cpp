#include <iostream>
#include <string>
#include <type_traits>
using namespace std;

template<typename T>
T str_to_num(const std::string& s)
{
  if constexpr (is_same_v<T, int>) {
    return std::stoi(s);
  } else if constexpr (is_same_v<T, long>) {
    return std::stol(s);
  } else if constexpr (is_same_v<T, long long>) {
    return std::stoll(s);
  } else if constexpr (is_same_v<T, double>) {
    return std::stod(s);
  }
}
//g++ 3_if.cpp -std=c++17
int main() {
    std::string s4 = "3.14159";
    double d = str_to_num<double>(s4);
    std::cout << "double: " << d << std::endl;
    return 0;
}