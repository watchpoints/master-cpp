#include <iostream>

class Foo
{
public:
     int number;
     Foo(int n)
     {
       number = n;
     }
};


int main() {
    Foo* ptr = new Foo(10);
    return 0;
}