#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Object.h"
#include "Exception.h"

#include "ListNode.h"
#include "List.h"

using namespace std;
using namespace MyLib;

static int g_count = 0;

class Test_traverse
{
public:
	Test_traverse() {}

	void operator () (int &e) const
	{
		cout << "value = " << e << ", count = " << g_count << endl;	
		g_count++;
	}

	~Test_traverse() {}

};

void test_sort()
{

	srand(time(NULL));

	int loop_count = rand() % 99999;
	//int loop_count = 1;
	cout << "loop_count = " << loop_count << endl;

	while (0 < loop_count--)
	{
		if (0 == loop_count % 10)
			cout << "loop_count = " << loop_count << endl;
		MyLib::List<int> l;
		int elem_count = rand() % 999;
		//cout << "elem count = " << elem_count << endl;
		for (int i = 0; i < elem_count; i++)
		{
			l.insertAsLast(rand());
		}
		//l.traverse(Test_traverse());
		//l.deduplicate();
		//cout << "disorder : " << l.disordered() << endl;
		l.sort();

		if (l.size() != elem_count)
		{
			cout << "elem count is not correct ..." << endl;
			break;
		}

		//cout << "size = " << l.size() << endl;
		//cout << "sort result : " << endl;
		//l.traverse(Test_traverse());

		//cout << "disorder : " << l.disordered() << endl;
		if (0 != l.disordered())
		{
			cout << "test failed ..." << endl;
			break;
		}
		else
			//cout << "test success ..." << endl;
			;
	}

}

void test_selectMax()
{

	MyLib::List<int> l;

	srand(time(NULL));

	for (int i = 0; i < 10; i++)
	{
		l.insertAsLast(rand() % 9992);
	}

	//l.traverse(Test_traverse());
	//l.deduplicate();
	//l.sort();
	l.traverse(Test_traverse());

	//MyLib::Posi(int) pos = l.selectMax(l.first(), l.size());
	MyLib::Posi(int) pos = l.selectMax();
	cout << "max = " << pos->m_data << endl;

	pos = pos->m_succ;
	if (l.valid(pos))
		cout << "succ data = " << pos->m_data << endl;
}

static void test_rank(void)
{

	MyLib::List<int> l;

	l.insertAsLast(1);

	Posi(int) pos = l.last();

	pos = pos->m_succ;
	pos = pos->m_succ;

	//cout << l.rank(pos) << endl;
}
#include "Vector.h"
static void test_vector_sort()
{
	
	srand(time(NULL));

	int loop_count = rand() % 9999;

	while (--loop_count > 0)
	{
		if (!(loop_count % 100))
			cout << "loop_count = " << loop_count << endl;

		MyLib::Vector<int> v;
		int elem_count = rand() % 9999;
		for (int i = 0; i < elem_count; i++)
		{
			Rank pos = rand() % (v.size() + 1);
			v.insert(pos, rand() % 1000);
		}

		//cout << "elem_count = " << elem_count << endl;
		//cout << "ori size = " << v.size() << endl;

		v.sort();

		if (0 != v.disordered())
		{
			cout << "sort failed..." << endl;
			break;
		}
		if (elem_count != v.size())
		{
			cout << "size is not correct, size = "  << v.size() << endl;
			break;
		}
	}
}

#include <omp.h>

bool loop_once(int elem_count)
{
	bool ret = true;
	MyLib::Vector<int> v;

	for (int i = 0; i < elem_count; i++)
	{
		MyLib::Rank pos = rand() % (v.size() + 1);
		v.insert(pos, rand() % 9999999);

	}
	
	v.sort();

	if (0 != v.disordered())
	{
		cout << "sort failed..." << endl;
		ret = false;
	}
	if (elem_count != v.size())
	{
		cout << "size is not correct, size = " << v.size() << endl;
		ret = false;
	}

	return ret;
}

void test_sort_parallel()
{
	srand(time(NULL));
	omp_set_num_threads(2);
	#pragma omp parallel
	for (int i = 0; i < 99999999; i++)
	{
		if (!loop_once(rand() % 99999))
			break;

	}

}

void test_reverse()
{
	MyLib::List<int> l;

	for (int i = 0; i < 10; i++)
	{
		l.insertAsLast(i);
	}
	
	l.traverse(Test_traverse());
	l.reverse_V3();

	l.traverse(Test_traverse());

}

int main(void)
{
	test_reverse();
	//test_selectMax();
	//test_sort();
	//test_vector_sort();
	//test_sort_parallel();
#if 0
	MyLib::List<int> l;

	l.insertAsLast(1);
	l.insertAsLast(5);
	l.insertAsLast(7);
	l.insertAsLast(7);
	l.insertAsLast(9);
	l.insertAsLast(9);
	l.insertAsLast(11);
	l.insertAsLast(13);
	l.insertAsLast(15);
	l.insertAsLast(21);
	l.insertAsLast(28);
	l.insertAsLast(28);
	l.insertAsLast(28);
	l.insertAsLast(66);
	l.insertAsLast(999);
	l.insertAsLast(999);
	//MyLib::Posi(int) pos = l.find(1);

	//printf("pos = %p\n", pos);
	//l.deduplicate();
	//cout << "old size = " << l.size() << endl;
	//l.uniquify();
	//cout << "size = " << l.size() << endl;

	l.traverse(Test_traverse());
	
	MyLib::Posi(int) tmp = l.search(13);
	MyLib::Posi(int) pos = l.search(28, tmp, 4);
	//MyLib::Posi(int) pos = l.search(1000);
	cout << l.valid(pos) << endl;
	cout << pos->data() << endl;
	pos = pos->m_succ;
	if (l.valid(pos))
	{
		cout << pos->data() << endl;
		pos = pos->m_succ->m_succ;
		if (l.valid(pos))
			cout << pos->data() << endl;
	}
#endif
	return 0;
}

#if 0
	
	MyLib::ListNode<int> n;
	struct : public Object
	{
		char reserved[sizeof(int)];
		MyLib::Posi(int) pred;
		MyLib::Posi(int) succ;
	} test_header;

	cout << "sizeof(n) = " << sizeof(n) << endl;

	cout << "sizeof(header) = " << sizeof(test_header) << endl;

#endif

