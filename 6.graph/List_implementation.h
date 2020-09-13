#pragma once

#ifndef __LIST_IMPLEMENTATION_H__
#define __LIST_IMPLEMENTATION_H__

#include <iostream>
using namespace std;

template < typename T >
List<T>::List()
{
    init();
}

template < typename T >
void List<T>::init(void)
{
    m_header = reinterpret_cast<Posi(T)> (&m_node_header);
    m_trailer = reinterpret_cast<Posi(T)> (&m_node_trailer);
    m_size = 0;
    m_header->m_pred = NULL;
    m_header->m_succ = m_trailer;
    m_trailer->m_succ = NULL;
    m_trailer->m_pred = m_header;
}

template < typename T >
T &List<T>::operator [] (Rank r) const
{
    /*位置r的合法性检查: 0 <= r < size*/
    if (0 <= r && r > m_size)
    {
        Posi(T) p = first();
 
        while (r > 0)
        {
            p = p->m_succ;  /*指向下一个*/
            r--;
        }

        return p->m_data;
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundException, "Index of List out of bound ...");
    }
}


template < typename T >
Rank List<T>::rank(Posi(T) p) const
{
	Rank ret = -1;/*header的秩就是-1*/
	Posi(T) start = m_header;

	/*从首节点一直遍历到尾节点*/
	while (NULL != start)
	{
		if (start == p)
		{
			/*找到了*/
			break;
		}
		ret++;
		start = start->m_succ;
	}
	/*ret的合法取值范围是[-1, size], 当ret = size + 1表明p不在列表中, ret == -1表明p是列表的header, ret == size表明p是列表的trailer*/
	return ret;
}

template < typename T >
Posi(T) List<T>::find(const T &e) const
{
	return find(e, m_size, m_trailer);
}

template < typename T >
Posi(T) List<T>::find(const T &e, int n, Posi(T) p) const
{
	Rank r = rank(p);
	Posi(T) ret = NULL;
	cout << "r = " << r << endl;
	/*0 <= n <= rank(p) <= size, 等于size时等于从末节点开始向前找*/
	if ((n >= 0) && (n <= r) && (r <= m_size))
	{
		while (n > 0)
		{
			p = p->m_pred;	/*向前找,不包括入参p的前n个*/
			if (e == p->m_data)
			{
				/*找到了*/
				ret = p;
				break;
			}
			n--;	
		}
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "Trying to find element in list with invalid params ...");
	}

	return ret;
}

template < typename T >
bool List<T>::valid(Posi(T) p) const
{
	return ((NULL != p) && (m_trailer != p) && (m_header != p));
}

template < typename T >
Posi(T) List<T>::find(const T &e, Posi(T) p, int n) const
{
	Rank r = rank(p);
	Posi(T) ret = NULL;

	/*
	 * 从p的后继开始,向后找元素值为e的节点,由此p可以是header,
	 * 然后p后面必须要有n个有效元素, 即rank(p) + n < size
	 */
	if ((n >= 0) && (r >= -1) && (r + n < m_size))/*不再需要调用valid接口,因为这样可以满足*/
	{
		while (n > 0)
		{
			p = p->m_succ;	/*向后找,不包括入参p的后n个*/
			if (e == p->m_data)
			{
				/*找到了*/
				ret = p;
				break;
			}
			n--;
		}
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "Trying to find element in list with invalid params ...");
	}

	return ret;
}

template < typename T >
Posi(T) List<T>::last() const
{
	return m_trailer->m_pred;
}

template < typename T >
Posi(T) List<T>::first() const
{
	return m_header->m_succ;
}

template < typename T >
Posi(T) List<T>::insertAsFirst(const T &e)
{
	Posi(T) ret = m_header->insertAsSucc(e);	/*作为头节点的后继插入即可*/
	++m_size;
	return ret;	
}

template < typename T >
Posi(T) List<T>::insertAsLast(const T &e)
{
	Posi(T) ret = m_trailer->insertAsPred(e);	/*作为末节点的前驱插入即可*/
	++m_size;
	return ret;
}

template < typename T >
Posi(T) List<T>::insertBefore(Posi(T) p, const T &e)
{
	/*TODO:检查这个节点p是否合法*/
	Posi(T) ret = p->insertAsPred(e);
	m_size++;

	return ret;
}

template < typename T >
Posi(T) List<T>::insertAfter(Posi(T) p, const T &e)
{
	Posi(T) ret = p->insertAsSucc(e);
	m_size++;

	return ret;
}

template < typename T >
void List<T>::copyNodes(Posi(T) p, int n)
{
	init();
	while (n > 0)
	{
		insertAsLast(p->m_data);
		p = p->m_succ;
		n--;
	}
}

template < typename T >
List<T>::List(Posi(T) p, int n)
{
	copyNodes(p, n);
}

template < typename T >
List<T>::List(const List<T> &l)
{
	copyNodes(l.first(), l.m_size);	
}

template < typename T >
List<T>::List(const List<T> &l, Rank r, int n)
{
	if (r + n > l.m_size)
		n = m_size - r;

	copyNodes(l[r], n);
}

template < typename T >
T List<T>::remove(Posi(T) p)
{
	Posi(T) toDel = p;
	T ret = toDel->m_data;

	toDel->m_pred->m_succ = toDel->m_succ;
	toDel->m_succ->m_pred = toDel->m_pred;

	/*先重建列表,然后在删除待删除节点,异常安全*/	
	m_size--;
	delete toDel;

	return ret;
}

template < typename T >
T List<T>::remove(Rank r)
{
	T ret;
	if ((0 <= r) && (r < m_size))
	{
		Posi(T) p = m_header->m_succ;

		while (0 < r--)
			p = p->m_succ;
		
		ret = remove(p);
	}
	else
	{
		THROW_EXCEPTION(IndexOutOfBoundException, "trying to remove element from list with invalid index ...");	
	}

	return ret;
}

template < typename T >
int List<T>::clear()
{
	int old_size = m_size;

	while (m_size > 0)
	{
		remove(m_header->m_succ);
	}

	return old_size;
}

template < typename T >
int List<T>::deduplicate()
{
	int old_size = m_size;

	if (m_size < 2)	/*规模小于2的自然不含重复元素*/
		return 0;

	/*采用与向量一样的无序去重操作即可,从第1个开始,从前面找相同的,如果有,就删除当前的,否则++*/
	Posi(T) p = m_header->m_succ;
	Rank r = 0;

	/*
	 * 以下迭代,每一次循环,都有不变性描述如下:
	 * 	当前节点p的所有前驱均互不相同
	 * 同时每一次迭代,问题规模减一,满足单调性,因此算法得证
	 */
	/*
	 * 循环次数为长度n,最坏情况是没有重复的,这样的话总需要1 + 2 + 3 + ... + n次,算术级数,复杂度为末项平方即 O(n^2)
	 * 最好情况就是都相同,这样p前驱永远只有一个元素
	 */
	while (m_trailer != p)
	{
		Posi(T) toDel = find(p->m_data, r, p);	/*find接口复杂度正比于长度r*/

		if (NULL != toDel)
			remove(toDel);	/*如果存在,保留当前节点,重复节点toDel删掉,需要常数阶的复杂度*/
		else
			r++;	/*如果不存在,则p指向下一个,这时候待查找区间长度就需要加一,也就是r要加一*/
		/*
		 * 与向量的不一样,这里要更新p的值,
		 * 因为在列表中不应该通过Rank访问对应的元素l[r],而需要通过位置p
		 * 因为列表的循秩访问效率太低了,因此不使用,
		 */

		p = p->m_succ;
	}

    return old_size - m_size;
}

template < typename T >
Rank List<T>::size() const
{
	return m_size;
}

template < typename T >
bool List<T>::empty() const
{
	return (0 == m_size);
}

template < typename T >
void List<T>::traverse(void (*visit)(T &e))
{
	for (Posi(T) p = m_header->m_succ; m_trailer != p; p = p->m_succ)
		visit(p->m_data);
}

template < typename T>
template < typename VST >
void List<T>::traverse(const VST &visit)
{

	for (Posi(T) p = m_header->m_succ; m_trailer != p; p = p->m_succ)
		visit(p->m_data);
}

template < typename T >
int List<T>::uniquify()
{
	int old_size = m_size;

	/*与有序向量的去重类似,采用双指针法*/
	Posi(T) i = m_header->m_succ;	/*从第一个开始,i指向l[0]*/
	Posi(T) j = i->m_succ;	/*j应当指向l[1]*/

	/*显然算法的时间复杂度取决于这个循环,j从1到size,是线性阶的时间复杂度O(n)*/
	for (; m_trailer != j; )
	{
		/*
		 * 比较下标j是否与i相等
		 * 如果相等,这时候应当先删掉j,然后让j重新指向i的后面一个,而不能简单自增1,因为列表物理上是不连续的
		 * 如果不相等,则将i移动到j的位置上
		 */
		if (i->m_data == j->m_data)
		{
			remove(j);
		}
		else
		{
			i = j;
		}
		/*每一次迭代,j总是i的后面一个*/
		j = i->m_succ;
	}
	
	return old_size - m_size;
}

template < typename T >
Posi(T) List<T>::search(const T &e, int n, Posi(T) p) const
{
	Rank r = rank(p);
	Posi(T) ret = NULL;

	/*
	 * 从p开始向前找n,找出其中最后一个不大于e的元素的位置
	 * 首先是n的值必须大于等于0,其次是p前面要有n个有效的前驱(visible)
	 * 此外,由于查找操作不包括p本身,因此允许p是trailer,因此支持整个列表的查找
	 */
	if (n >= 0 && r >= 0 && r >= n - 1 && r <= m_size)
	{
		ret = p->m_pred;
		while (0 < n--)
		{
			if (ret->m_data <= e)
			{
				/*找到*/
				/*
				 * 正确性也是显而易见的:
				 * 因为列表是有序的,而且迭代从后向前迭代,于是乎,遇到的第一个小于等于e的节点,这个节点的所有后继必然都大于e,这个节点的所有有效前驱必然都小于等于e,那么这个节点就是所求
				 */
				break;
			}
			ret = ret->m_pred;
		}
	}
	else
	{
		cout << "here" << endl;
		cout << "n = " << n << ", r = " << r << ", m_size = " << m_size << endl;
		THROW_EXCEPTION(InvalidParameterException, "Trying to search element from list with invalid params ...");
	}
	
	/* 返回值分析:如果列表中存在e,那返回值为对应的最后一个不大于e的元素的位置,
	 * 当查找失败时,整个查找操作将持续n次,最终ret停留在l[rank(p) - n]的位置上,如果是整个列表查询,则这时候ret实际上就是m_header
	 * 换而言之,当查找失败时返回查找区间的左边界的前驱(可能是非法的,非法的情况就是返回header)
	 */
	return ret;
}

template < typename T >
Posi(T) List<T>::search(const T &e) const
{
	return search(e, m_size, m_trailer);
}

template < typename T >
Posi(T) List<T>::search(const T &e, Posi(T) p, int n) const
{
	Posi(T) ret = NULL;
	Rank r = rank(p);
	
	/*保证n是合法的,以及p后面有n个有效的后继,位置p必须从首元素开始*/
	if ((n >= 0) && (r >= 0) && (r + n < m_size) && (valid(p)))
	{
		ret = p->m_succ;
		while (0 < n--)
		{
			if (e <= ret->m_data)
			{
				/*找到立即返回,符合接口设计语义*/
				/*合法性是显然的,因为列表是有序的,整个迭代从前向后遍历,找到的第一个大于等于查找元素的节点必然就是第一个不小于e的节点位置,这个节点的所有有效前驱的元素值必然小于e,从这个节点开始的所有节点必然大于等于e,于是乎当前节点就是所求*/
				break;
			}
			ret = ret->m_succ;
		}
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "Searching forwad in list with invalid params, input param must match [rank(p) >= 0 &&rank(p) + n < size]");	
	}
	
	/*
	 * 返回位置p之后的第一个不小于e的元素的位置
	 * 当查找成功时,返回秩最小的位置
	 * 当查找失败时,返回查找区间的右区间的后继所在的位置(整区间查找失败时返回末节点trailer的位置)
	 */
	return ret;
}

#if 0
template < typename T >
void List<T>::insertionSort(Posi(T) p, int n)
{
	/*实质就是对  l[p, p + n)进行插入排序*/
	/*参数检查:检查p是否合法,从秩来说, 必须要满足0 <= p < p + n <= size*/
	Rank r = rank(p);
	//cout << "r = " << r << endl;

	if (valid(p) && n >= 0 && r >= 0 && r + n <= m_size)
	{
		/*
		 * 插入排序的思想:
		 * 将待排序的区间分为两段,前面一段是已经有序的,后面一段是待排序的,开始时前面一段为空
		 * 通过迭代,将后面一段的首元素插入到前面一段的有序位置上,迭代完成后整个区间自然就是有序的了
		 * 当然插入时是有要求的,需要插入到前面一段的最后一个不大于当前待插入元素值的后面,这个通过已有的接口可以轻松实现
		 */
		for (int i = 0; i < n; i++)	/*平均分布的情况下,整体O(n^2)*/
		{
			//cout << "i = " << i << endl;
			Posi(T) pos = search(p->m_data, i, p);	/*前面一段是有序的,通过search接口查找,最坏情况(也就是完全逆序,每次都要走完i次)O(i)的复杂度*/
			insertAfter(pos, p->m_data);/*插入到pos的后面,常数复杂度*/
			/*将当前元素从后面的区间中删掉,然后指向下一个*/
			p = p->m_succ;
			remove(p->m_pred);
		}	
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "insertion-sort with invalid params ...");
	}
}
#endif

template < typename T >
void List<T>::insertionSort(Posi(T) p, int n)
{
	/*实质就是对  l[p, p + n)进行插入排序*/
	/*参数检查:检查p是否合法,从秩来说, 必须要满足0 <= p < p + n <= size*/
	Rank r = rank(p);
	//cout << "r = " << r << endl;

	if (valid(p) && n >= 0 && r >= 0 && r + n <= m_size)
	{
		/*
		 * 插入排序的思想:
		 * 将待排序的区间分为两段,前面一段是已经有序的,后面一段是待排序的,开始时前面一段为空
		 * 通过迭代,将后面一段的首元素插入到前面一段的有序位置上,迭代完成后整个区间自然就是有序的了
		 * 当然插入时是有要求的,需要插入到前面一段的最后一个不大于当前待插入元素值的后面,这个通过已有的接口可以轻松实现
		 */
		for (int i = 0; i < n; i++)	/*平均分布的情况下,整体O(n^2)*/
		{
			//cout << "i = " << i << endl;
			Posi(T) pos = search(p->m_data, i, p);	/*前面一段是有序的,通过search接口查找,最坏情况(也就是完全逆序,每次都要走完i次)O(i)的复杂度*/
			/*将当前元素从后面的区间中删掉,然后指向下一个*/
			Posi(T) toDel = p;
			p = p->m_succ;
			removeFromList(toDel);
			/*然后将这个节点插入到pos的后面*/
			insertAfter(pos, toDel);
		}	
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "insertion-sort with invalid params ...");
	}
}

template < typename T >
void List<T>::sort()
{
	/*当长度小于2时不需要排序*/
	if (m_size >= 2)
		sort(m_header->m_succ, m_size);
}

template < typename T >
void List<T>::sort(Posi(T) p, int n)
{
	int index = rand() % 3;

	/*抄袭书里边的写法,实际上是按照需要选择合适的排序算法,这样写可以测试所有的排序方法*/
	switch (index)
	{
		case 0:
			insertionSort(p, n);
			break;
		case 1:
			selectionSort(p, n);
			break;
		default:
			mergeSort(p, n);
	}
	//insertionSort(p, n);
	//selectionSort(p, n);
	//mergeSort(p, n);
}

template < typename T >
Posi(T) List<T>::selectMax(Posi(T) p, int n) const
{
	Posi(T) ret = NULL;
	Rank r = rank(p);
	/*合法性检查:p必须是visible的, 然后就是p包括自己的总长不能超过有效长度*/
	if (valid(p) && r >= 0 && r + n <= m_size)
	{
		Posi(T) cur = p;	/*初始化指向第一个,也就是p自己*/
		ret = p;	/*返回值初始化为第一个*/
		/*从第2个开始,跟max比较,即可*/
		for (int i = 1; i < n; i++)
		{
			cur = cur->m_succ;	/*从1到第n - 1个*/
			if (cur->m_data >= ret->m_data)
			{
				/*更新max*/
				ret = cur;
			}
		}
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "Tring to select max element from list with invalid params ...");	
	}

	return ret;
}

template < typename T >
Posi(T) List<T>::selectMax() const
{
	return selectMax(m_header->m_succ, m_size);
}
#if 0
template < typename T >
void List<T>::selectionSort(Posi(T) p, int n)
{
	/*合法性检查,选择排序合法输入区间为[0, size), 且 p + n <= size*/
	Rank r = rank(p);
	if (valid(p) && n >= 0 && r >= 0 && r + n <= m_size)
	{
		/*
		 * 选择排序的思想是:
		 * 将待排序区间分为两段,前面一段是无序的,后面一段是有序的,初始状态后面一段节点个数为0
		 * 每次迭代,通过selectMax从前面区间中挑选最大的节点,然后将这个节点插入到后面区间的首位,
		 * 选择排序的正确性是显而易见的,同时,通过规范selectMax接口,使得整个选择排序是稳定的
		 */
		
		/*注意head的取值,不能指向p,因为有可能p就是最大的,那这时候p就需要移动了,因此head应当指向p的前驱,p的前驱在本次排序过程中是不涉及的*/
		Posi(T) head = p->m_pred;
		
		/*同样的tail也应当选择为l[rank(p) + n], 这个节点也是不涉及排序的*/
		Posi(T) tail = p;
		for (int i = 0; i < n; i++)
			tail = tail->m_succ;
		
		while (n > 1)
		{
			Posi(T) max = selectMax(head->m_succ, n);
			T elem = remove(max);
			insertBefore(tail, elem);

			/*tail还需要前移一格,因为插入了个新的节点到它的前面了*/
			tail = tail->m_pred;

			n--;
		}	
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "selection-sort with invalid params ...");
	}
}
#endif

template < typename T >
void List<T>::selectionSort(Posi(T) p, int n)
{
	/*合法性检查,选择排序合法输入区间为[0, size), 且 p + n <= size*/
	Rank r = rank(p);
	if (valid(p) && n >= 0 && r >= 0 && r + n <= m_size)
	{
		/*
		 * 选择排序的思想是:
		 * 将待排序区间分为两段,前面一段是无序的,后面一段是有序的,初始状态后面一段节点个数为0
		 * 每次迭代,通过selectMax从前面区间中挑选最大的节点,然后将这个节点插入到后面区间的首位,
		 * 选择排序的正确性是显而易见的,同时,通过规范selectMax接口,使得整个选择排序是稳定的
		 */
		
		/*注意head的取值,不能指向p,因为有可能p就是最大的,那这时候p就需要移动了,因此head应当指向p的前驱,p的前驱在本次排序过程中是不涉及的*/
		Posi(T) head = p->m_pred;
		
		/*同样的tail也应当选择为l[rank(p) + n], 这个节点也是不涉及排序的*/
		Posi(T) tail = p;
		for (int i = 0; i < n; i++)
			tail = tail->m_succ;
		
		while (n > 1)
		{
			Posi(T) max = selectMax(head->m_succ, n);
			//T elem = remove(max);
			Posi(T) toDel = removeFromList(max);
			insertBefore(tail, toDel);

			/*tail还需要前移一格,因为插入了个新的节点到它的前面了*/
			tail = tail->m_pred;

			n--;
		}	
	}
	else
	{
		THROW_EXCEPTION(InvalidParameterException, "selection-sort with invalid params ...");
	}
}

template < typename T >
int List<T>::disordered() const
{
	int ret = 0;

	/*平凡情况:长度不足二的必然有序*/	
	if (m_size < 2)
		return 0;

	Posi(T) start = m_header->m_succ->m_succ;/*非平凡情况必然可以访问到L[1]*/
	for (int i = 1; i < m_size; i++)
	{
		if (start->m_pred->m_data > start->m_data)
			ret++;
		
		start = start->m_succ;	
	}

	return ret;
}

/*实现两个列表的二路归并,一个列表是当前列表的p位置起,长度为n所构成的列表,另一个列表是l的q位置起,长度为m的子列表*/
template < typename T >
void List<T>::merge(Posi(T) &p, int n, List<T> &l, Posi(T) q, int m)
{
	/*内部接口,参数合法性不再做检查,上一层接口使用前需保证参数的合法性,待排序节点个数要合法,两个归并的列表不能重叠*/
	/*类似于向量的归并排序最终版,这里的意图就是将l中从q开始(包括q)的m个节点合并到当前列表中*/
	Posi(T) head = p->m_pred;

	while (m > 0)
	{
#if 1	/*自己实现的逻辑,与邓公的类似,不过顺序反了罢了*/
		if ((n <= 0) || (p->m_data > q->m_data))
		{
			/*当前列表所有待排序节点都遍历完或者节点q的元素值小于p的元素值,将q从l中取出来作为p的前驱插入到当前列表中*/
			Posi(T) toDel = q;
			q = q->m_succ;	/*q移向下一个节点*/
			m--;
			/*插入*/
			//p->insertAsPred(toDel->m_data);	/*一开始这么写的,然后发现内存泄漏了,是因为不能这么弄,这样弄不会修改m_size的值,然后后面的remove会修改,导致出错了,查了很久...*/
			//insertBefore(p, toDel->m_data);
			//l.remove(toDel);
			removeFromList(toDel);
			insertBefore(p, toDel);
		}
		else
		{
			/*p中还有节点待排序而且当前节点p的元素值大于等于节点q的元素值,这时候p继续后移*/
			p = p->m_succ;
			n--;
			/*这里增加一个检查,检查下一轮比较的节点p跟q的位置是否相等,如果是可以直接跳出循环,意图就是在列表归并排序时可以进一步的减少运算量*/
			if (q == p)
				break;
		}
#else
		/*邓公示例代码中的写法*/
		if ((0 < n) && (p->m_data <= q->m_data))
		{
			if (q == (p = p->m_succ))
			{
				break;
			}
			n--;
		}
		else
		{
			Posi(T) toDel = q;
			insertBefore(p, q->m_data);
			q = q->m_succ;
			l.remove(toDel);
			//insertBefore(p, l.remove((q = q->m_succ)->m_pred));
			m--;
		}
#endif
	}

	p = head->m_succ;	/*这里返回归并后的起点*/
}

template < typename T >
void List<T>::merge(List<T> &l)
{
	/*这是一个public接口,意思是将另一个列表l归并到当前列表中,使用二路归并的方式*/
	/*需要做参数检查*/
	if (this != &l)
	{
		/*二路归并要求两路输入必须有序*/
		if (0 != disordered() || 0 != l.disordered())
		{
			THROW_EXCEPTION(InvalidOperationException, "Trying to mering two out-of-ordered list ...");
		}
		merge(m_header->m_succ, m_size, l.first(), l.m_size);
	}
	/*对自己做归并等于不用做*/
}

template < typename T >
void List<T>::mergeSort(Posi(T) &p, int n)
{
	/*protected接口不做参数合法性检查,调用此接口的必须保证参数合法性*/
	/*归并排序,与向量的差不多,先砍半递归的做,最后两个有序的再调用二路归并接口做合并即可*/
	/*平凡情况,节点个数小于2必然有序*/

	if (n < 2)
	{
		return;
	}
	int middle = n >> 1;

	/*列表的循秩访问...*/
	Posi(T) q = p;
	for (int i = 0; i < middle; i++)
		q = q->m_succ;

	/*假设p的秩为r, 前半段为 l[r, r + middle), 后半段为 l[r + middle, r + n)*/
	mergeSort(p, middle);

	/*后半段归并,前半段长度为middle,总体有n个,后半段个数为n - middle*/
	mergeSort(q, n - middle);

	/*最后对两段有序的列表做二路归并*/
	merge(p, middle, *this, q, n - middle);
}


template < typename T >
Posi(T) List<T>::removeFromList(Posi(T) p)
{
	Posi(T) toDel = p;

	toDel->m_pred->m_succ = toDel->m_succ;
	toDel->m_succ->m_pred = toDel->m_pred;

	m_size--;

	return toDel;
}

template < typename T >
Posi(T) List<T>::insertBefore(Posi(T) p, Posi(T) n)
{
	/*为了效率考虑,且本接口是private的,不做检查*/
	p->m_pred->m_succ = n;
	n->m_pred = p->m_pred;
	p->m_pred = n;
	n->m_succ = p;

	m_size++;

	return n;
}

template < typename T >
Posi(T) List<T>::insertAfter(Posi(T) p, Posi(T) n)
{
	p->m_succ->m_pred = n;
	n->m_succ = p->m_succ;
	p->m_succ = n;
	n->m_pred = p;

	m_size++;

	return n;
}

template < typename T >
void Swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template < typename T >
void List<T>::reverse()
{
	/*列表元素翻转版本二,实现思路是将所有节点(包括header和trailer)的pred和succ指针对调*/
	/*经过上面的步骤后实际上列表就整个的翻转了,于是乎header和trailer也被翻转了,所以在最后就要将header和trailer对调回来*/
	Posi(T) p = m_header;

	while (NULL != p)
	{
		Swap(p->m_pred, p->m_succ);
		p = p->m_pred;	/*原来访问下一个是succ,对调以后就是pred*/
	}

	Swap(m_header, m_trailer);
}

/*书里边最后列表翻转使用的是这里的实现,但是这样在泛指类型T较为复杂时两个节点元素值的交换可能会非常耗时,这个实现与向量的翻转实现方法一致*/
template < typename T >
void List<T>::reverse_V3()
{
	Posi(T) p = m_header->m_succ;
	Posi(T) q = m_trailer->m_pred;
	/*列表元素翻转的版本三*/
	/*
	 * 实现思路如下:
	 * 第一轮迭代将l[0]与l[size - 1]的元素值进行交换
	 * 第二轮将l[1]与l[size - 2]节点处的元素值进行交换
	 * ......
	 * 直到两个节点位置的指针之间元素小于2为止,这个算法的不变性和单调性是显然的*/
	for (int i = 1; i < m_size; i += 2)	/*这里i从1开始挺牛逼的,就能筛选出size <= 1的平凡情况了,而且迭代次数也是正确的*/
	{
		Swap(p->m_data, q->m_data);
		p = p->m_succ;
		q = q->m_pred;
	}
}

template < typename T >
List<T>::~List()
{
	clear();
}

#endif
