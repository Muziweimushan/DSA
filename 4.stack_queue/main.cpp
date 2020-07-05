#include <iostream>
#include "Stack.h"

using namespace std;

/*进制转换,进制转换实际上就是将原数据不断整除进制,然后将余数逆序输出即可,其中逆序输出通过栈完成*/
void hex_convertion(MyLib::Stack<char> &s, int64_t n, int base)
{
	const char digit[] = {'0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};	/*这里最多支持16进制转换*/

	while (n > 0)
	{
		int remainder = static_cast<int>(n % base);	/*余数*/
		s.push(digit[remainder]);
		n = n / base;
	}
}

bool stack_permutation_judge(MyLib::Stack<int> A, MyLib::Vector<int> &B)
{
	MyLib::Stack<int> S;	/*辅助栈*/

	bool loop = true;
	bool ret = true;
	int k = 0;

	int count = 0;

	for (k = 0; k < B.size() && loop; k++)
	{
		while (S.empty() || B[k] != S.top())
		{
			if (!A.empty())
			{
				S.push(A.pop());
			}
			else
			{
				/*此时A已经是空栈,说明不能借助S将A元素转移至B*/
				loop = false;
				break;
			}
			count++;
		}
		/*我们是模拟A的一次栈混洗过程,到此处说明当前需要将辅助栈的栈顶元素pop出来转移至B中,因此这里应当是将S的栈顶元素弹出即可*/
		/*判断一下是否是相等的,因为上面的while循环有可能终止*/
		if (B[k] == S.top())
			S.pop();
	}

	cout << "count = " << count << endl;

	/*到这里,如果B确实是A的一个栈混洗,辅助栈S应当为空,A也为空,因为A中所有元素都借助S转移至B了嘛*/
	ret = (A.empty() && S.empty());

	return ret;
}

void test_judge()
{
	MyLib::Vector<int> v;

	srand(time(NULL));
	for (int i = 0; i < 3; i++)
	{
		v.insert(rand());	
	}
	
	MyLib::Stack<int> A;
	for (int i = v.size(); i > 0; )
		A.push(v[--i]);
	
	MyLib::Vector<int> B = A;

	B.permute();

	cout << stack_permutation_judge(A, B) << endl;
}

/*括号匹配*/
bool parenthesis(const char *s)
{
	bool ret = true;
	MyLib::Stack<char> S;
	/*括号匹配的思路就是线性扫描输入，如果遇到左括号就压入栈中,如果遇到右括号则与栈顶元素进行比较,一旦不一致,则说明括号不匹配,如果一致,则将这对匹配的括号从计算过程删去,继续向后扫描,当整个输入扫描完成后,如果辅助栈大小为0,则说明匹配,否则也是不匹配的*/
	
	int len = strlen(s);

	for (int i = 0; i < len && ret; i++)
	{
		switch (s[i])
		{
			case '(' :
			case '[' :
			case '{' :
			{
				/*左括号入栈即可*/
				S.push(s[i]);
				break;
			}
			case ')' :
			{
				if (S.empty() || (S.top() != '('))
				{
					/*不匹配！*/
					ret = false;
				}
				else
				{
					/*找到一对括号,将它们pop出来即可*/
					S.pop();
				}
				break;
			}

			case ']' :
			{
				if (S.empty() || (S.top() != '['))
				{
					/*不匹配！*/
					ret = false;
				}
				else
				{
					/*找到一对括号,将它们pop出来即可*/
					S.pop();
				}
				break;
			}
			case '}' :
			{
			
				if (S.empty() || (S.top() != '{'))
				{
					/*不匹配！*/
					ret = false;
				}
				else
				{
					/*找到一对括号,将它们pop出来即可*/
					S.pop();
				}
				break;
			}
			default:
			{
				/*其他不关心的字符不管*/
				break;
			}
		
		}
	}

	if (ret)
	{
		/*到此处如果ret为真,说明整个输入扫描完了,再来检查辅助栈是否为空*/
		ret = S.empty();
	}
	
	return ret;
}


int main(void)
{
	char str[] = "(sahf[fa]{dsga}";

	cout << parenthesis(str) << endl;


#if 0	/*测试栈混洗*/	
	test_judge();

	return 0;
#endif

#if 0	/*测试进制转换*/
	MyLib::Stack<char> s;

	hex_convertion(s, 72727271212321515, 16);

	while (s.size() > 0)
		cout << s.pop();

	cout << endl;
	return 0;
#endif
}
