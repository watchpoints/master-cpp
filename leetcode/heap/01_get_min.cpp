#include <iostream>
#define SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
using namespace std;
//Minimum Element in a Max Heap in C++
int getMinElement(int *heap, int n){
   int minElement = heap[n / 2];
   for (int i = n / 2 + 1; i < n; ++i) {
      minElement = min(minElement, heap[i]);
   }
   //time complexity: O(n)
   return minElement;
}
//https://www.tutorialspoint.com/minimum-element-in-a-max-heap-in-cplusplus
int main(){
   int heap[] = {120, 90, 100, 70, 75, 80, 60, 25, 40, 35};
   cout << "Min value: " << getMinElement(heap, SIZE(heap)) << "\n";
   return 0;
}