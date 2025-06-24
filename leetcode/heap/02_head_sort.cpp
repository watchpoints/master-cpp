// HeapSort.cpp : 定义控制台应用程序的入口点。
/*大根堆排序算法的基本操作：
① 初始化操作：将R[1..n]构造为初始堆；
② 每一趟排序的基本操作：将当前无序区的堆顶记录R[1]和该区间的最后一个记录交换，然后将新的无序区调整为堆(亦称重建堆)。
注意：
①只需做n - 1趟排序，选出较大的n - 1个关键字即可以使得文件递增有序。
②用小根堆排序与利用大根堆类似，只不过其排序结果是递减有序的。
堆排序和直接选择排序相反：在任何时刻，堆排序中无序区总是在有序区之前，
且有序区是在原向量的尾部由后往前逐步扩大至整个向量为止。*/
#include "stdafx.h"
#include <iostream>  

using namespace std;

//生成大根堆  
void HeapAdjust(int SortData[], int StartIndex, int Length)
{
	while (2 * StartIndex + 1 < Length)
	{
		int MaxChildrenIndex = 2 * StartIndex + 1;
		if (2 * StartIndex + 2 < Length)
		{
			//比较左子树和右子树，记录最大值的Index  
			if (SortData[2 * StartIndex + 1] < SortData[2 * StartIndex + 2])
			{
				MaxChildrenIndex = 2 * StartIndex + 2;
			}
		}
		if (SortData[StartIndex] < SortData[MaxChildrenIndex])
		{
			//交换i与MinChildrenIndex的数据  
			int tmpData = SortData[StartIndex];
			SortData[StartIndex] = SortData[MaxChildrenIndex];
			SortData[MaxChildrenIndex] = tmpData;
			//堆被破坏，需要重新调整  
			StartIndex = MaxChildrenIndex;
		}
		else
		{
			//比较左右孩子均大则堆未破坏，不再需要调整  
			break;
		}
	}
}

//堆排序  
void HeapSortData(int SortData[], int Length)
{
	int i = 0;

	//将Hr[0,Length-1]建成大根堆  
	for (i = Length / 2 - 1; i >= 0; i--)
	{
		HeapAdjust(SortData, i, Length);
	}

	for (i = Length - 1; i > 0; i--)
	{
		//与最后一个记录交换  
		int tmpData = SortData[0];
		SortData[0] = SortData[i];
		SortData[i] = tmpData;
		//将H.r[0..i]重新调整为大根堆  
		HeapAdjust(SortData, 0, i);
	}
}

//TestCase  
int main()
{
	int SortData[] = { 12,36,24,85,47,30,53,91 };

	HeapSortData(SortData, 8);

	for (int i = 0; i < 8; i++)
	{
		cout << SortData[i] << " ";
	}
	cout << endl;

	return 0;
}

//https://www.cnblogs.com/chengxiao/p/6129630.html