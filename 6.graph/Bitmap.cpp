#include <cstring>
#include <cstdio>
#include "Bitmap.h"

#include <iostream>

using namespace std;
using namespace MyLib;

Bitmap::Bitmap(int n)
{
	init(n);
}

Bitmap::Bitmap(const char *const file, int n)
{
	FILE *fp = NULL;

	if (NULL == file)
		THROW_EXCEPTION(InvalidParameterException, "initing Bitmap with invalid params ...");

	init(n);
	
	fp = fopen(file, "r");
	if (NULL == fp)
	{
		char msg[256];
		snprintf(msg, sizeof(msg), "opening file %s for Bitmap failed ...\n", file);
		THROW_EXCEPTION(InvalidOperationException, msg);	
	}
	size_t read_len = fread(m_array, sizeof(char), m_size, fp);	
	fclose(fp);
	fp = NULL;

	if (m_size != (int)read_len)
	{
		delete [] m_array;
		m_array = NULL;
		THROW_EXCEPTION(InvalidOperationException, "Reading bitmap info failed ...");
	}
}

void Bitmap::init(int n)
{
	m_size = (n + 7) / 8;/*向上取整*/

	m_array = new char[m_size];

	if (NULL != m_array)
	{
		memset(m_array, 0, m_size);	
	}
	else
	{
		THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for bitmap inner array space ...");
	}
}

void Bitmap::expand(int n)
{
	/*要注意对应容量为n bit的位图(n是8的倍数), 只能访问到[0, n - 1], 因此这里是大于而不是大于等于*/
	if (m_size * 8 > n)
		return;

	cout << "we need to expand ..." << endl;

	int old_size = m_size;
	char *old_array = m_array;

	init(n * 2);/*类比vector,创建一个2倍容量的*/

	memcpy(m_array, old_array, old_size);

	delete [] old_array;
}

void Bitmap::set(int k)
{
	expand(k);
	/*位运算*/
	m_array[k >> 3] |= (0x80 >> (k & 0x7));
}

void Bitmap::clear(int k)
{
	expand(k);

	/*一样的位运算*/
	m_array[k >> 3] &= ~(0x80 >> (k & 0x7));
}

bool Bitmap::test(int k)
{
	expand(k);

	return (m_array[k >> 3] & (0x80 >> (k & 0x7)));
}

char *Bitmap::bits2string(int n)
{
	char *ret = NULL;
	expand(n - 1);	/*只访问到n-1的位置*/

	ret = new char[n + 1];
	ret[n] = '\0';
	if (NULL != ret)
	{
		for (int i = 0; i < n; i++)
		{
			ret[i] = (test(i)) ? '1' : '0';
		}	
	}
	else
	{
		THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for bits2string output ...");	
	}

	return ret;
}

void Bitmap::dump(const char * const file)
{
	if (NULL != file)
	{
		FILE *fp = fopen(file, "w");

		if (NULL != fp)
		{
			size_t write_len = fwrite(m_array, sizeof(char), m_size, fp);
			fclose(fp);
			fp = NULL;
			if (m_size != (int)write_len)
			{
				THROW_EXCEPTION(InvalidOperationException, "dumping bitmap info to file failed...");
			}
		}
		else
		{
			char err_msg[256];
			snprintf(err_msg, sizeof(err_msg), "opening file %s for dumping bitmap info failed ...", file);
			THROW_EXCEPTION(InvalidOperationException, err_msg);
		}
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "dumping bitmap to NULL file ...");
	}	
}

Bitmap::~Bitmap()
{
	delete [] m_array;
}

