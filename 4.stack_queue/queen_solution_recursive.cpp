#include <iostream>
#include <cstdio>

#include "Vector.h"

using namespace std;

static int g_solve_count = 0;

static void print_solution(const MyLib::Vector<int> solution)
{
	printf("find solution[%d] : \n", g_solve_count);

	for (int i = solution.size(); i > 0; i--)
	{
		for (int j = 0; j < solution.size(); j++)
		{
			if (solution[i - 1] == j)
				printf("[*]");
			else
				printf("[ ]");
		}
		printf("\n");
	}

	printf("\n\n");
	return;
}

static bool collide(const MyLib::Vector<int> solution)
{
	int new_x = solution.size() - 1;
	int new_y = solution[new_x];
	bool ret = false;

	/*检查第k行的这个皇后是否与前k-1行都不发生冲突*/
	for (int i = 0; i < solution.size() - 1; i++)
	{
		int old_x = i;
		int old_y = solution[i];

		if (new_x == old_x)
		{
			/*行是否相等,这个是不会发生滴*/
			ret = true;
			break;
		}
		else if (new_y == old_y)
		{
			/*是否处于同一列*/
			ret = true;
			break;
		}
		else if (new_y - old_y == new_x - old_x)
		{
			/*是否处理对角线上*/
			ret = true;
			break;
		}
		else if (new_y - old_y == old_x - new_x)
		{
			/*是否处于反对角线上*/
			ret = true;	
			break;
		}
	}
	return ret;
}

/*
 * 	查找在棋盘的第k行放置皇后的可行方案
 *	前面k-1行的皇后坐标存放在solution中 	
 */
static void placeQueen(MyLib::Vector<int> solution, int n, int k)
{
	if (k != solution.size())
	{
		/*不对...*/
		THROW_EXCEPTION(MyLib::InvalidParameterException, "size of vector is not correct ...");
	}

	/*处理平凡情况:如果已经找到一种解,输出解法并直接返回即可*/
	if (n == solution.size())
	{
		g_solve_count++;
		//print_solution(solution);	
	}
	else
	{
		/*在此处先insert效率目测高一些*/
		solution.insert(solution.size(), 0);
		/*在第k行,从第0列开始,依次放下皇后,然后检查与前k - 1行的已放置皇后是否有冲突,如果有,继续在下一列放皇后,如果没有则可以递归到下一行,直到进入平凡情况*/
		for (int i = 0; i < n; i++)	/*遍历n列*/
		{
			solution.put(i, k);	/*在第k行第i列放置皇后,然后进行检查*/
			if (!collide(solution))
			{
				/*木有冲突,递归处理下一行*/
				placeQueen(solution, n, k + 1);
			}
			else
			{
				/*有冲突,说明第i列不ok,继续第i+1列*/
			}
		}
	}

}


/*N皇后通用解法,递归版本*/
void queen_solution_V1(int n)
{
	MyLib::Vector<int> solution;
	
	try
	{	
		placeQueen(solution, n, 0);
	}
	catch (const MyLib::Exception &obj)
	{
		cout << "place queen failed : "	 << obj.location() << obj.message() << endl;
		return;
	}
	catch (...)
	{
		cout << "place queen failed ..." << endl;
		return;
	}

	cout << "find solution count = " << g_solve_count << endl;

	return;
}
