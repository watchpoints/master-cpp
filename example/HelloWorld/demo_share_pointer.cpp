
#include <iostream>
#include <iostream>
#include <memory>
//#include <mutex>
#include <thread>
using namespace std;
struct Base
{
    Base() { std::cout << "Base::Base()\n"; }
 
    // Note: non-virtual destructor is OK here
    ~Base() { std::cout << "Base::~Base()\n"; }
};


void thread_read(std::shared_ptr<Base> sp1)
{
    std::shared_ptr<Base> lp = sp1; // 线程 A 的局部变量
    std::cout << "read thead use count: " << sp1.use_count() << std::endl;
}

void thread_write(std::shared_ptr<Base> sp1)
{
    std::shared_ptr<Base> sp3 = std::make_shared<Base>(); //// 线程 B 的局部变量
    sp1=sp3; //reset 
    std::cout << "write thead use count: " <<sp1.use_count() << std::endl;
}

//g++ -g -Wall -std=c++11 demo_share_pointer.cpp
//g++ -std=c++11 -pthread -o test demo_share_pointer.cpp
int main()
{
	
  std::shared_ptr<Base> sp1 = std::make_shared<Base>(); //线程之间共享的 shared_ptr
  std::cout << "use count: " << sp1.use_count() << std::endl;
  
  shared_ptr<Base> sp2 = sp1;
  std::cout << "use count: " << sp1.use_count() << std::endl;
  
  shared_ptr<Base> sp3 = sp2;
  std::cout << "use count: " << sp1.use_count() << std::endl;

  /** 
	Base::Base()
	use count: 1
	use count: 2
	use count: 3
	Base::~Base()
 **/
 
  std::thread t1{thread_read, sp1}, t2{thread_write, sp1};
  
 
  t1.join();
  t2.join();

  std::cout << "All threads completed,\n";

  return 0;
}
