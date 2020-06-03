#pragma once

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "Object.h"
#include "Exception.h"

using namespace std;

namespace MyLib
{

/*比特图类,又称为位图*/
/*
 * 我们使用char型数组来存储各个bit的信息,每个字节可以存储8个bit的信息
 * 第k位的元素与char数组中位置的映射关系为:
 * 所求元素在数组中的位置 =  k >> 3
 * 第k位元素所在的bit位(与vector类似称为秩) = k & 0x7, 其实就相当于取余
 * 假设数组中对应元素为 c = array[k >> 3]; 第k位元素的值为 c & (0x80 >> (k & 0x7))
 *
 */
class Bitmap : public Object
{
public:
	/*创建一个n位的位图*/
	Bitmap(int n = DEFAULT_BITMAP_SIZE);
	/*从文件中读出位图信息,据说是留给散列表使用的*/
	Bitmap(const char * const file, int n = DEFAULT_BITMAP_SIZE);

	/*将位图中第k位设置为true*/
	void set(int k);
	/*将位图中第k位设置为false*/
	void clear(int k);
	/*获取位图中第k位的值*/
	bool test(int k);

	/*将前n位转换为字符串, [0, n -1]*/
	char *bits2string(int n);

	/*将位图扩展为k位*/
	void expand(int n);

	/*将位图信息导出至文件,以便根据此文件对新位图做批量初始化,别问,写就完了...*/
	void dump(const char * const file);

	~Bitmap();

protected:
	/*书中是这样的,不知道为什么弄成protected的*/
	/*初始化一个k bit的位图*/
	void init(int k);

private:
	enum { DEFAULT_BITMAP_SIZE = 8 };	/*默认情况下是一个8bit的位图*/

	char *m_array;/*内部维护实际位图数据所用到的数组空间*/
	int m_size;	/*内部数组大小*/
};


}

#endif
