
#include <iostream>
using namespace std;

class Node
{
public:
	int m_data;
};//别忘记分号


struct A 
{
  void* operator new(size_t sz) 
  {
      cout << "A::operator new, size = " << sz << endl;
      return ::operator new(sz);
  }
  
  void operator delete(void* p) 
  {
      cout << "A::operator delete" << endl;
      ::operator delete(p);
  }
};