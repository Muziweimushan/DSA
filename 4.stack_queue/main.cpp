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


int main(void)
{
	MyLib::Stack<char> s;

	hex_convertion(s, 72727271212321515, 16);

	while (s.size() > 0)
		cout << s.pop();

	cout << endl;
	return 0;
}
