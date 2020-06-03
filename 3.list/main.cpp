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

	//int loop_count = rand() % 99999;
	int loop_count = 1;
	cout << "loop_count = " << loop_count << endl;

	while (0 < loop_count--)
	{
		if (0 == loop_count % 100)
			cout << "loop_count = " << loop_count << endl;
		MyLib::List<int> l;
		int elem_count = rand() % 10;
		cout << "elem count = " << elem_count << endl;
		for (int i = 0; i < elem_count; i++)
		{
			l.insertAsLast(rand());
		}
		//l.traverse(Test_traverse());
		//l.deduplicate();
		//cout << "disorder : " << l.disordered() << endl;
		l.sort();

		cout << "size = " << l.size() << endl;
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

int main(void)
{
	//test_selectMax();
	test_sort();

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

