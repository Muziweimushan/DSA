#include <iostream>
#include <cstdio>
#include <time.h>

#include "Vector.h"
#include "Object.h"
#include "Exception.h"
#include "Fib.h"

using namespace std;
using namespace MyLib;

void test(const Vector<int> &v)
{

	cout << "v[0] = " << v[0] << endl;
}

void print_vector(Vector<int> &v)
{
	cout << "vector capacity = " << v.capacity() << endl;
	cout << "vector size = " << v.size() << endl;
	for (int i = 0; i < v.size(); i++)
	{
		cout << "v[" << i << "] = " << v[i] << endl;	
	}
}


#include <algorithm>
void set_vector_order(Vector<int> &v)
{
	int *array = new int[v.size()];

	srand(time(NULL));
	for (int i = 0; i < v.size(); i++)
	{
		v[i] = rand() % 100;
	}
	sort(array, array + v.size());

	for (int i = 0; i < v.size(); i++)
	{
		v[i] = array[i];	
	}

	delete [] array;
}

void set_rand_value(int *v, int size)
{
	srand(time(NULL));
	for (int i = 0; i < size; i++)
	{
		v[i] = rand() % 10000;
	}
}

void set_rand_value_sort(int *v, int size)
{
	srand(time(NULL));
	for (int i = 0; i < size; i++)
	{
		v[i] = rand() % 1000000;
	}
	sort(v, v + size);
}

void set_rand_value(Vector<int> &v)
{
	srand(time(NULL));
	for (int i = 0; i < v.size(); i++)
	{
		v[i] = rand() % 10000;
	}
}

template < typename T >
void Swap(T &a, T &b)
{
	T c = a;

	a = b;
	b = c;
}

template<>
void Swap<int> (int &a, int &b)
{
	//cout << "here!" << endl;

	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}

/*array[lo, hi)单趟扫描交换,将[lo,hi)中最大的那个元素怼到array[hi - 1]的位置*/
bool bubble(int *array, int lo, int hi)
{
	bool bSorted = true;

	while (++lo < hi)
	{
		/*比较array[lo]和array[lo - 1]*/
		if (array[lo - 1] > array[lo])
		{
			Swap(array[lo], array[lo - 1]);
			bSorted = false;
		}
	}

	return bSorted;
}

void bubble_sort(int *array, int lo, int hi)
{
	while (!bubble(array, lo, hi))
	{
		hi--;	
	}
}

/*做二路有序向量的合并*/
void merge(int *array, int *helper, int lo, int mi, int hi)
{
	int len_helper = hi - lo;
	//int *helper = new int[len_helper];
	int *my_helper = helper + lo;	


	int i = lo;
	int j = mi;
	int k = 0;

	/*每次对两个数组的首元素进行比较,小的就扔到helper辅助空间中,然后指向下一个元素*/
	/*遍历直到到达其中一个数组的最后元素,就不能再继续了,否则就与解法不一致了*/
	while ((i < mi) && (j < hi))
	{
		if (array[i] < array[j])
		{
			my_helper[k] = array[i];
			i++;
			k++;
		}
		else
		{
			my_helper[k] = array[j];
			j++;
			k++;
		}
	}

	/*还有一个数组没遍历完,这个数组剩余的元素一定比helper中的都要大于或等于,拷贝到helper中就好了*/
	while (i < mi)
	{
		my_helper[k] = array[i];
		k++;
		i++;	
	}

	while (j < hi)
	{
		my_helper[k] = array[j];
		k++;
		j++;
	}

	for (int i = 0; i < len_helper; i++)
	{
		array[lo + i] = my_helper[i];
	}

	//delete [] helper;
}

/*做二路有序向量的合并*/
void merge(int *array, int lo, int mi, int hi)
{
	/*
	 *array[lo, mi)是有序的
	 *array[mi, hi)也是有序的,通过本函数使得[lo,hi)是有序的
	 */
#if 0
	int len_1st_array = mi - lo;/*第一个数组长度*/
	int len_2nd_array = hi - mi;

	int *pa = array + lo; /*第一个数组首元素*/
	int *pb = array + mi;
#endif
	int len_helper = hi - lo;
	int *helper = new int[len_helper];
	
	int i = lo;
	int j = mi;
	int k = 0;

	/*每次对两个数组的首元素进行比较,小的就扔到helper辅助空间中,然后指向下一个元素*/
	/*遍历直到到达其中一个数组的最后元素,就不能再继续了,否则越界了*/
	while ((i < mi) && (j < hi))
	{
		if (array[i] < array[j])
		{
			helper[k] = array[i];
			i++;
			k++;
		}
		else
		{
			helper[k] = array[j];
			j++;
			k++;
		}
	}

	/*还有一个数组没遍历完,这个数组剩余的元素一定比helper中的都要大于或等于,拷贝到helper中就好了*/
	while (i < mi)
	{
		helper[k] = array[i];
		k++;
		i++;	
	}

	while (j < hi)
	{
		helper[k] = array[j];
		k++;
		j++;
	}

	for (int i = 0; i < len_helper; i++)
	{
		array[lo + i] = helper[i];
	}

	delete [] helper;
}

void merge_sort(int *array, int *helper, int lo, int hi)
{

	if (hi - lo <= 1) return;

	int mid = (hi + lo) >> 1;

	merge_sort(array, helper, lo, mid);
	merge_sort(array, helper, mid, hi);

	merge(array, helper, lo, mid, hi);
}

void merge_sort(int *array, int lo, int hi)
{
	int *helper = new int[hi - lo];

	merge_sort(array, helper, lo, hi);

	delete [] helper;

#if 0
	if (hi - lo <= 1) return;

	int mid = (hi + lo) >> 1;

	merge_sort(array, lo, mid);
	merge_sort(array, mid, hi);

	/*经过以上2步, array[lo,mid)是有序的, array[mid,hi)是有序的,调用二路归并*/
	merge(array, lo, mid, hi);
#endif
}

#include <unistd.h>
#include "Bitmap.h"
#include "Stack.h"

namespace MyLib
{
	/*将十进制数n转换为base进制数*/
	void convert(Stack<char> &s, int64_t n, int base)
	{
		static char digit[] = {'0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};/*根据实际情况继续扩展即可*/

		while (n > 0)
		{
			int remainer = static_cast<int>(n % base);
			s.push(digit[remainer]);
			n = n / base;
		}
	}

}

namespace MyLib
{
	/*表达式括号匹配检查*/
	bool paren(const char *str)
	{
		if (NULL == str)
			THROW_EXCEPTION(InvalidParameterException, "NULL ptr ...\n");

		Stack<char> s;

		for (int i = 0; '\0' != str[i]; i++)
		{
			switch (str[i])
			{
				/*如果是左括号就压入栈中*/
				case '[' :
				case '(' :
				case '{' :
				{
					s.push(str[i]);
					break;	
				}
				/*如果是右括号,则栈顶元素必然是对应的左括号,否则就是不匹配*/
				case ']' :
				{
					if (s.empty() || s.pop() != '[')
						return false;
					break;
				}
				case ')' :
				{
					if (s.empty() || s.pop() != '(')
						return false;
					break;
				}
				case '}' :
				{
					if (s.empty() || s.pop() != '{')
						return false;
					break;
				}
				default:
					break;
			}	
		}

		/*遍历完整个字符串后,然后栈中没有左括号了,说明是匹配的*/
		return s.empty();
	}
}

int main(void)
{

	cout << MyLib::paren("(sfa{fasaf[ safaf ]afa(afa)f})") << endl;
#if 0
	int count = rand();
	int64_t n = count;

	MyLib::Stack<char> s;
	MyLib::convert(s, n, 10);

	cout << "count = " << count << endl;
	
	int capacity = s.size();
	for (int i = 0; i < capacity; i++)
	{
		cout << s.pop() << endl;
	}
#endif
	//cout << s.empty() << endl;
#if 0
	Bitmap b("./1.map", 56);

	char *result = b.bits2string(56);
	printf("%s\n", result);
	//cout << "result = " << result << endl;

	delete [] result;
//#else
	Bitmap b(10);

	for (int i = 0; i < 56; i++)
	{
		if (!(i % 4))
			b.set(i);
	}

	char *result = b.bits2string(16);
	printf("%s\n", result);
	//cout << "result = " << result << endl;

	delete [] result;

	b.dump("./1.map");
#endif
#if 0
	int array[1000];

	set_rand_value(array, 1000);
	//set_rand_value_sort(array, 20);
	//sleep(1);
	//set_rand_value_sort(array + 20, 20);

	for (int i = 0; i < 1000; i++)
	{
		//cout << "array[" << i << "] = " << array[i] << endl;
	}

	cout << endl << endl << endl << "after sorting ..." << endl << endl << endl;

	merge_sort(array, 0, 1000);

	for (int i = 0; i < 1000; i++)
	{
		cout << "array[" << i << "] = " << array[i] << endl;
	}

#endif
#if 0
	int array[100];

	set_rand_value(array, 100);

	bubble_sort(array, 0, 100);

	for (int i = 0; i < 100; i++)
	{
		cout << "array[" << i << "] = " << array[i] << endl;
	}
#endif
#if 0
	Fib f(20000);

	cout << f.get() << endl;

	//for (int i = 0; i < 10; i++)
	while (1)
	{
		int tmp = f.prev();
		if (tmp > 0)
			cout << tmp << endl;
		else
			break;
	}
#endif
	//Vector<int> v1(13, 13, 1);

	//set_vector_order(v1);

	//cout << v1.search(86, 0, v1.size()) << endl;
	//print_vector(v1);
	//v1.insert(5, 5);
	//for (int i = 0; i < 3; i++)
	//	v1[i + 5] = 111;
	
	//cout << "\n\n\n\n\nafter insert :" << endl;
	//print_vector(v1);
	//v1.unsort(1, 4);
	//v1.unsort(1, 4);
	//v1.unsort(1, 4);
	//v1.unsort(1, 4);

	//print_vector(v1);

	//cout << v1.find(1, v1.size(), 1) << endl;

	//v1.remove(0, 4);
	//v1.deduplicate();
    //cout << v1.disordered() << endl;
	//print_vector(v1);
#if 0
	v1[1] = 1;
	v1[2] = 2;
	v1[3] = 3;
	v1[4] = 4;

	Vector<int> v2(1, 4, v1);/*拷贝v1[1]到v1[4],区间的元素到v2中,前闭后开*/

	for (int i = 0; i < v2.size(); i++)
	{
		cout << v2[i] << endl;
	}
	cout << "v2.capacity = " << v2.capacity() << endl;
	cout << "v2.size = " << v2.size() << endl;
#if 0
	cout << "after insert : " << endl;
	v2.insert(v2.size() + 1, 5);

	for (int i = 0; i < v2.size(); i++)
	{
		cout << v2[i] << endl;
	}
	cout << "v2.capacity = " << v2.capacity() << endl;
	cout << "v2.size = " << v2.size() << endl;
#endif
	Vector<int> v3(2, v1);
	for (int i = 0; i < v3.size(); i++)
	{
		cout << v3[i] << endl;
	}
	cout << "v3.capacity = " << v3.capacity() << endl;
	cout << "v3.size = " << v3.size() << endl;

	Vector<int> v4(5, 5);
	v4 = v2;
	for (int i = 0; i < v4.size(); i++)
	{
		cout << v4[i] << endl;
	}
	cout << "v4.capacity = " << v4.capacity() << endl;
	cout << "v4.size = " << v4.size() << endl;
#endif
#if 0
	InvalidOperationException e("test!", __func__, __LINE__);
	cout << "message = " << e.message() << endl;
	cout << "location = " << e.location() << endl;

	try
	{
		THROW_EXCEPTION(InvalidOperationException, "test for throw Exception");	
	}
	catch (const InvalidOperationException &e)
	{
		cout << "here!" << endl;
		cout << e.message() << endl;
		cout << e.location() << endl;
	}
	catch (...)
	{
	
	}
#endif
	//throw e;
#if 0
	MyLib::Vector<int> v;

	v[0] = 1;

	cout << "size = " << v.size() << endl;
	cout << "capacity = " << v.capacity() << endl;

	cout << "v[0] = " << v[0] << endl;

	test(v);
#endif
	return 0;
}
