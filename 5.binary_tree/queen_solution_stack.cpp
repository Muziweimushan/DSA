#include <iostream>
#include <cstdio>

#include "queen.h"
#include "Stack.h"

using namespace std;

static int g_try_count = 0;
static int g_solve_count = 0;

class PrintQueen
{
public:

	PrintQueen(int n)
	{
		m_n = n;
	}

	void operator () (const Queen &q) const
	{
		int i = 0;
		for (i = 0; i < q.m_y; i++)
			printf("[ ]");
		printf("[*]");
		
		for (++i; i < m_n; i++)
			printf("[ ]");
		printf("\n");
	}

private:
	int m_n;	/*列个数,也就是N皇后的N是多少*/
};

static void print_solution(const MyLib::Stack<Queen> &solution, int n)
{

	printf("--\n");
	const_cast<MyLib::Stack<Queen> &>(solution).traverse(PrintQueen(n));
	printf("\n");
}

static void placeQueen(int N)
{
	MyLib::Stack<Queen> solution;	/*辅助栈,用于记录皇后的位置*/

	Queen q(0, 0);
	
	while (q.m_x > 0 || q.m_y < N)	/*继续迭代的条件应当是queen的x和y坐标不要同时非法就可以*/
	{
		/*检查当前皇后的列坐标,是否已经出界,如果是就回去上一行*/
		if (q.m_y >= N)
		{
			/*这时候需要回溯,因此需要将上一行的皇后的位置恢复,实际上就是辅助栈的栈顶元素,然后列坐标++*/
			q = solution.pop();
			q.m_y++;	/*从下一列开始*/
		}
		else
		{
			/*在当前行不断尝试*/
			/*使用vector的find操作,同时Queen类通过==操作符重载来实现判断当前行的皇后位置是否合法*/
			while ((q.m_y < N) && (solution.find(q) >= 0))
			{
				q.m_y++;
				g_try_count++;	
			}
			/*循环结束后,要么找了一个合法位置,要么就是当前皇后的列坐标已经出界*/
			if (q.m_y < N)
			{
				/*说明找到了一个不冲突的皇后*/
				solution.push(q);
				/*如果此时辅助栈大小为N,说明找到了一个解*/
				if (solution.size() == N)
				{
					g_solve_count++;
					//print_solution(solution, N);
				}
				/*移动到下一行,而且从列坐标0开始*/
				q.m_x++;
				q.m_y = 0;
			}
			else
				;	/*else分支情况在下一轮迭代中进入if分支...*/
		}
	}
}

/*N皇后问题通用解,利用栈做回溯,迭代版本*/
void queen_solution_V2(int N)
{
	try
	{
		placeQueen(N);
	}
	catch (const MyLib::Exception &obj)
	{
		cout << "queen solution failed : " << obj.location() << obj.message() << endl;
		return;
	}
	catch (...)
	{
		cout << "queen solution failed ..." << endl;
		return;
	}

	cout << "queen solution v2 : find count = " << g_solve_count << endl;
}
