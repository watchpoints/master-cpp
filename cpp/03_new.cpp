
class Foo
{
public:
     int number;
     Foo(int n)
     {
       number = n;
     }
};

//g++ 03_new.cpp -lstdc++
int main() {
    Foo* ptr = new Foo(10);
    return 0;
}