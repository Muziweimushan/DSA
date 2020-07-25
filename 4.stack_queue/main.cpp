#include <iostream>
#include <cstdlib>
#include <math.h>

#include "Stack.h"
#include "Exception.h"

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

static bool readNumber(char *&s, MyLib::Stack<double> &stack)
{
	char *end = NULL;
	char *start = s;

	double number = strtod(start, &end);
	if (start == end)
	{
		printf("read number failed ...\n");
		return false;
	}

	cout << "get double = " << number << endl;

	stack.push(number);
	s += (end - start);
	return true;

}

typedef enum
{
	OPERATOR_TYPE_ADD = 0x0,	/*加法*/
	OPERATOR_TYPE_SUB = 0x1,	/*减法*/
	OPERATOR_TYPE_MUL = 0x2,	/*乘法*/
	OPERATOR_TYPE_DIV = 0x3,	/*除法*/
	OPERATOR_TYPE_FAC = 0x4,	/*阶乘*/
	OPERATOR_TYPE_POW = 0x5,	/*乘方*/
	OPERATOR_TYPE_L_P = 0x6,	/*左括号*/
	OPERATOR_TYPE_R_P = 0x7,	/*右括号*/
	OPERATOR_TYPE_TERMINATOR = 0x8,	/*结束符*/
	OPERATOR_TYPE_CNT
}OPERATOR_TYPE;

typedef enum
{
	OPERATOR_PRIORITY_GREATER_THAN = 0x0,	/*栈顶操作符优先级更高*/
	OPERATOR_PRIORITY_LESS_THAN = 0x1,	/*栈顶元素优先级低于当前元素*/
	OPERATOR_PRIORITY_EQUAL = 0x2,		/*栈顶元素与当前元素优先级相同*/
	OPERATOR_PRIORITY_INVALID = 0x3,	/*非法...*/
	OPERATOR_PRIORITY_CNT
}OPERATOR_PRIORITY;

static const char c_priority_table[OPERATOR_TYPE_CNT][OPERATOR_TYPE_CNT] = 
{
/*				当前元素						*/
/*		  '+'  '-' '*'	 '/'  '!'  '^'	'('  ')'  '\0'	*/
/* --  '+' */	{ '>', '>', '<', '<', '<', '<', '<', '>', '>' },

/* |   '-' */	{ '>', '>', '<', '<', '<', '<', '<', '>', '>' },

/* 栈  '*' */	{ '>', '>', '>', '>', '<', '<', '<', '>', '>' },
	
/* 顶  '/' */	{ '>', '>', '>', '>', '<', '<', '<', '>', '>' },

/* 运  '!' */	{ '>', '>', '>', '>', '>', '>', ' ', '>', '>' },

/* 算  '^' */	{ '>', '>', '>', '>', '<', '>', '<', '>', '>' },

/* 符  '(' */	{ '<', '<', '<', '<', '<', '<', '<', '=', ' ' },

/* |   ')' */	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },

/* --  '\0' */	{ '<', '<', '<', '<', '<', '<', '<', ' ', '=' }
};

static int operator_to_rand(const char &c)
{
	switch (c)
	{
		case '+' : return OPERATOR_TYPE_ADD;
		case '-' : return OPERATOR_TYPE_SUB;
		case '*' : return OPERATOR_TYPE_MUL;
		case '/' : return OPERATOR_TYPE_DIV;
		case '!' : return OPERATOR_TYPE_FAC;
		case '^' : return OPERATOR_TYPE_POW;
		case '(' : return OPERATOR_TYPE_L_P;
		case ')' : return OPERATOR_TYPE_R_P;
		case '\0' : return OPERATOR_TYPE_TERMINATOR;
		default : return OPERATOR_TYPE_CNT;
	}
}

static OPERATOR_PRIORITY getPriBtw(const char &top_optr, const char &cur_optr)
{
	OPERATOR_PRIORITY ret = OPERATOR_PRIORITY_CNT;
	int index1 = operator_to_rand(top_optr);
	int index2 = operator_to_rand(cur_optr);

	if ((index1 < OPERATOR_TYPE_CNT) && (index2 < OPERATOR_TYPE_CNT))
	{
		char c = c_priority_table[index1][index2];
		switch (c)
		{
			case '>' : { ret = OPERATOR_PRIORITY_GREATER_THAN; break; }
			case '<' : { ret = OPERATOR_PRIORITY_LESS_THAN; break; }
			case '=' : { ret = OPERATOR_PRIORITY_EQUAL; break; }
			case ' ' : { ret = OPERATOR_PRIORITY_INVALID ; break; }
		}
	}

	return ret;
}

/*计算n的阶乘*/
static int64_t factorial(int n)
{
	int64_t ret = 1;

	while (n > 1)
	{
		ret *= n;
		n--;
	}

	return ret;
}

double calculate(char optr, double opand)
{
	double ret = 0;
	switch (optr)
	{
		case '!':
		{
			ret = factorial(static_cast<int>(opand));
			break;
		}
		default:
		{
			/*目前仅支持阶乘运算是单目的*/
			THROW_EXCEPTION(MyLib::ArithmeticException, "Not supported calculation ...");
			break;
		}
	}

	return ret;
}

double calculate(char optr, double opand1, double opand2)
{
	double ret = 0.0;

	switch (optr)
	{
		case '+' :
		{
			ret = opand1 + opand2;
			break;
		}
		case '-' :
		{
			ret = opand1 - opand2;
			break;
		}
		case '*' :
		{
			ret = opand1 * opand2;
			break;
		}
		case '/' :
		{
			/*检查除数是否为0*/
			double delta = opand2 - 0.0;
			const double epsilon = 0.00000000000001;
			if ((delta > -epsilon) && (delta < epsilon))
			{
				THROW_EXCEPTION(MyLib::ArithmeticException, "divisor could not be 0!");
			}
			else
			{
				ret = opand1 / opand2;
			}
			break;
		}
		case '^' :
		{
			ret = pow(opand1, opand2);
			break;
		}
		default:
		{
			THROW_EXCEPTION(MyLib::ArithmeticException, "Not supported calculation ...");
			break;
		}
	
	}

	return ret;
}

double evaluate(char *s, char *RPN)
{
	MyLib::Stack<double> stack_operand;	/*操作数栈*/
	MyLib::Stack<char> stack_operator;	/*操作符栈*/
	bool loop = true;
	double ret = 0.0;
	/*输入字符串的结尾是\0,先向操作符栈中压入一个\0用于匹配结尾*/
	stack_operator.push('\0');

	while (!stack_operator.empty() && loop)
	{
		/*线性扫描每一个输入字符,如果当前字符是数字,也就是操作数,将其存入操作数栈*/	
		if (isdigit(*s))
		{
			if (!readNumber(s, stack_operand))
			{
				/*读入操作数失败*/
				loop = false;
				break;
			}
			/*操作数同样串接到RPN后面即可*/

		}
		else
		{
			/*否则就说明是操作符,将当前操作符与栈顶元素进行优先级比较*/
			OPERATOR_PRIORITY priority = getPriBtw(stack_operator.top(), *s);
			switch (priority)
			{
				case OPERATOR_PRIORITY_GREATER_THAN :
				{
					/*操作符栈顶元素优先级更高,意味着可以执行当前栈顶操作符对应的运算了*/
					/*除了阶乘外其他都需要两个操作数*/
					char optr = stack_operator.pop();	/*获取当前要执行哪种计算*/
					if ('!' == optr)
					{
						if (stack_operand.size() >= 1)
						{
							double opand = stack_operand.pop();
							stack_operand.push(calculate(optr, opand));
						}
						else
						{
							/*表达式有问题,操作数栈为空*/	
							loop = false;
						}
					
					}
					else
					{
						/*例如要计算2+3,操作数栈顶元素是3,后面的才是2*/
						if (stack_operand.size() >= 2)
						{
							double opand2 = stack_operand.pop();
							double opand1 = stack_operand.pop();
							stack_operand.push(calculate(optr, opand1, opand2));
						}
						else
						{
							/*表达式有问题*/
							loop = false;
						}
					}
					break;
				}
				case OPERATOR_PRIORITY_LESS_THAN :
				{
					/*栈顶符号优先级低于当前符号,将这个符号入栈*/
					stack_operator.push(*s);
					s++;
					break;
				}
				case OPERATOR_PRIORITY_EQUAL:
				{
					/*相等情况对应于左右括号或者是首尾结束符匹配情况,这时候两个符号之间的运算都已经全部计算完成了,因此此处仅需要将栈顶符号pop出即可*/
					stack_operator.pop();
					s++;	/*当前符号也已经被处理过了,s需要指向下一个字符*/
					break;
				}
				default:
				{
					/*其他情况都属于表达式不正确*/
					loop = false;
					break;
				}
			}
		}
	}

	cout << "loop = " << loop << endl;
	cout << "size = " << stack_operand.size() << endl;

	if (loop && 1 == stack_operand.size())
	{
		/*计算成功*/
		/*检查操作数栈长度是否为1*/
		ret = stack_operand.pop();
	}
	else
	{
		THROW_EXCEPTION(MyLib::InvalidParameterException, "input expression is invalid ...");
	}

	return ret;
}


extern void queen_solution_V1(int n);

int main(void)
{

	queen_solution_V1(8);
#if 0	/*测试中缀表达式计算*/
	char expression[] = "0!+12+34*56+7+89";
	try
	{
		cout << evaluate(expression) << endl;
	}
	catch (const MyLib::Exception &obj)
	{
		cout << obj.location() << obj.message() << endl;
	}
	catch(...)
	{
	
	}
#endif
#if 0
	
	char str[] = "(sahf[fa]{dsga}";

	cout << parenthesis(str) << endl;
#endif

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
