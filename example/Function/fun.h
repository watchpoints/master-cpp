#include <iostream>

using namespace std;
int add(int a , int b  = 0, int c = 0);

int main()
{
	std::cout << add(1, 2, 3) << std::endl;
	getchar();
	return 0;
}

int add(int a, int b, int c)
{
	return a + b + c;
}
