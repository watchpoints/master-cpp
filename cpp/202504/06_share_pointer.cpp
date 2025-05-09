#include <iostream>
#include <memory>
#include <vector>
using namespace std;
 
class MM 
{
public:
	MM(string name="",int age=0):name(name),age(age){}
	void print() 
	{
		cout << name << "\t" << age << endl;
	}
	~MM() 
	{
		cout << "调用析构函数" << endl;
	}
protected:
	string name;
	int age;
};
void testTwo() 
{
	// cout <<"-----------------------------------" << endl;
	// shared_ptr<MM> p(new MM("name1", 19));
	// p->print();
	/*****************vector弊端*****************/
	// vector<MM*> p1;
	// p1.push_back(new MM("name", 20));
	// //p1.pop_back();
	// p1.clear();
	//可以看到pop和clear函数都不会自动调用析构函数---->当vector操作自定义类型指针的时候
	/*******************************************/
	//比较常用的用法--->工作中
	vector<shared_ptr<MM>> arr;
	shared_ptr<MM> p2(new MM("name"));
	p2->print();
	arr.push_back(p2);
}
int main(){
 
    testTwo();
    return 0;
}