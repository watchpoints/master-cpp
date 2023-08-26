#include <iostream>
#include <type_traits>
#include <functional>
using namespace std;

template<class T>
class Info
{
public:
  template < typename fun_t =std::function<int()>
           >
	void func(fun_t m)
	{
		cout<<"this is Info " << m()<< endl;
	}
};
template <typename T>
class Info2:public Info<T>
{

};
int main()
{
	Info2<int> a;
	a.func([](){return 10; });
	return 0;
}