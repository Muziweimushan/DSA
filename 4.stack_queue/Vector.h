#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cstdlib>
#include "Object.h"
#include "Exception.h"
#include "Fib.h"

#include <iostream>

using namespace std;

namespace MyLib
{

template < typename T >
class Vector : public Object
{
	/*定义向量的秩,实际是C++的基本类型int*/
	typedef int Rank;
public:
	/*内部空间容量设定为c,并将里边的[0,s)的元素初始化为v*/
	Vector(int c = DEFAULT_VECTOR_CAPACITY, int s = 0, const T &v = 0)
	{
		//cout << "Vector(const T&v, int c, int s), c = " << c << " s = " << s << endl;

		m_capacity = c;
		m_elem = new T[m_capacity];
		if (NULL != m_elem)
		{
			for (m_size = 0; m_size < s; m_size++)
			{
				m_elem[m_size] = v;
			}
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for inner space ...");
		}
	}

	/*使用入参v的[lo, hi)的数据来初始化向量*/
	Vector(Rank lo, Rank hi, const Vector<T> &v)
	{
		/*检查入参*/
		if ((0 > lo) || (lo >= hi) || (hi >= v.m_size))
		{
			THROW_EXCEPTION(InvalidParameterException, "Input Invalid parameters ...\n");
		}
		
		copyFrom(lo, hi, v.m_elem);
	}

	/*使用v中秩为0到n的单元来依次初始化向量*/
	Vector(Rank n, const Vector<T> &v)
	{
		if (n > 0 && n <= v.m_size)
		{
			copyFrom(0, n, v.m_elem);	
		}
		else
		{
			THROW_EXCEPTION(InvalidParameterException, "Input Invalid parameters ...\n");
		}
	}
	
	/*以数组v下标为lo到hi的数据元素([lo,hi),前闭后开)来依次初始化向量*/
	Vector(Rank lo, Rank hi, T *v)
	{
		if ((0 > lo) || (lo >= hi))
		{
			THROW_EXCEPTION(InvalidParameterException, "Input Invalid parameters ...\n");
		}
		
		copyFrom(lo, hi, v);
	}

	Vector(Rank n, T *v)
	{
		copyFrom(0, n, v);
	}

	Vector(Vector &v)
	{
		copyFrom(0, v.m_size, v.m_elem);	
	}

	Vector &operator = (const Vector &v)
	{
		/*避免自拷贝*/
		if (this != &v)
		{
			if (m_elem) { delete [] m_elem; }
			copyFrom(0, v.m_size, v.m_elem);
		}
		return *this;	
	}
	
	T &operator [] (Rank pos)
	{
		/*0 <= pos < size*/
		if ((pos < 0) || (pos >= m_size))
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "Accessing vector element with invalid index ...");
		}
		return m_elem[pos];
	}

	const T & operator [] (Rank pos) const
	{
		//cout << "const operator [] ..." << endl;
		if ((pos < 0) || (pos >= m_size))
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "Accessing vector element with invalid index ...");
		}
		return m_elem[pos];	
	}

    T &get(Rank r)
    {
        if (r < 0 || r >= m_size)
        {
            THROW_EXCEPTION(IndexOutOfBoundException, "Tring to get element from vector with invalid index ..."); 
        }

        return m_elem[r];
    }

    /*将向量中秩为r的单元值设置为e*/
    void put(const T &e, Rank r)
    {
        if (r < 0 || r >= m_size)
        {
            THROW_EXCEPTION(IndexOutOfBoundException, "Setting invlid elem ..."); 
        } 

        m_elem[r] = e;
    }

	/*将任意的元素e插入到向量中秩为r的单元中,返回元素e所在的秩*/
	Rank insert(Rank r, const T &e)
	{
		int ret = -1;
		if (r >= 0 && r <= m_size)
		{
			expand();/*检查看看是不是要扩容*/
			Rank pos = m_size;
			while (pos > r)
			{
				m_elem[pos] = m_elem[pos - 1];
				pos--;
			}
			m_elem[pos] = e;
			m_size++;
			ret = r;
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "insert element into vector with invalid rank ...");	
		}

		return ret;
	}

	/*固定插入到向量的末尾*/
	Rank insert(const T &e)
	{
		return insert(m_size, e);
	}

	int size() const { return m_size; }
	int capacity() const { return m_capacity; }
	void clean()
	{
		m_size = 0;
	}

	static void permute(Vector<T> &v)
	{
		/*从最后一个单元开始,每次从v[0]-v[k - 1]中选取一个元素与v[k]进行交换,直到遍历完整个向量*/
		for (int i = v.size(); i > 0; i--)
		{
			Swap(v[i - 1], v[rand() % i]);/*这个写法太牛逼了,从书中抄过来滴*/
		}
	}

	/*置乱整个向量中的元素*/
	void permute(void)
	{
		permute(*this);
	}

	/*等概率置乱区间为[lo, hi)的向量元素*/
	void unsort(Rank lo, Rank hi)
	{
		if (lo < 0 || hi < lo || hi >= m_size) 
		{
			THROW_EXCEPTION(InvalidParameterException, "unsorting vector with invalid params ...");	
		}

		T *V = m_elem + lo;
		for (int i = hi - lo; i > 0; i--)
		{
			Swap(V[i - 1], V[rand() % i]); /*将V[i - 1]与v[0, i - 1)中某个值进行随机交换*/
		}
	}

	/*在无序向量的区间[lo, hi)中查找元素e*/
	/*如果查找成功,返回对应的秩序,如果向量中有多个值为e的单元,约定统一返回满足要求的秩的最大值*/
	/*如果向量中没有对应单元,返回值为 - 1*/
	Rank find(Rank lo, Rank hi, const T &e)
	{
		Rank ret = -1;
		/*0 <= lo < hi <= size*/
		if (0 <= lo && lo < hi && hi <= m_size)
		{
			/*自后向前查找*/
			while ((--hi >= lo) && (e != m_elem[hi]))
			{
				;
			}
			//cout << "hi = " << hi << " lo = " << lo << endl;
			ret = (hi < lo) ? -1 : hi;
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "find element in vector with invaild index ...");
		}

		return ret;
	}

	Rank find(const T &e)
	{
		if (m_size <= 0)
			return -1;
		else
			return find(0, m_size, e);
	}
	
	/*删除向量区间[lo, hi)的元素*/
	/*返回值指明删除了多少个元素*/
	int remove(Rank lo, Rank hi)
	{
		/*0 <= lo <= hi <= size*/
		if ((lo < 0) || (hi > m_size))
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "removing elements from vector with invalid index ...");
		}
		/*
		 *	实现方法,直接将从V[hi,size)的数据拷贝到v[lo]开始的位置即可,这样时间复杂度与(size - hi)一致
		 */

		if (hi > lo) /*lo == hi的情况实际就是啥都不用干*/
		{
			while (hi < m_size)
			{
				m_elem[lo++] = m_elem[hi++];
			}
			m_size = lo;/*这样来计算size值...牛逼,抄就完了*/
			//检查看看要不要缩容
			shrink();
		}

		return hi - lo;
	}

	/*删除秩为r的元素*/
	T remove(Rank r)
	{
		T ret;
		/*0 <= r < m_size*/
		if ((r >= 0) && (r < m_size))
		{
			ret = m_elem[r];
			/*
			*   调用删除区间接口即可
			*	注意这里的设计是将删除单个元素作为区间删除的一个特例
			*	而不将区间删除设计为循环(hi - lo)次调用删除单个,是因为如果单独删除的话,每次都需要拷贝后面的元素
			*	使得时间复杂度变成 (hi - lo) * (size - hi)
			*/
			remove(r, r + 1);
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "Removing single element from vector with invalid index ...");	
		}

		return ret;
	}
	
	/*在无序向量中删除重复元素,时间复杂度为O(n^2)*/
	int deduplicate(void)
	{
		int old_size = m_size;
		Rank start = 1;//从下标为1开始

		while (start < m_size)
		{
			/*
			 * 	在向量的[0, start)区间找是否存在与v[start]一样的,
			 * 	如果有,说明v[start]处的元素是重复的,干掉它
			 *	否则就移动下标
			 */
			(find(0, start, m_elem[start]) < 0) ?
				/* ++start : (cout << "removing " << start << endl, remove(start));*/
				++start : remove(start);
		}

		return m_size - old_size;
	}
	
	/*
	 * 遍历函数
	 * 模板类型VST必须重载operator (T) const, visit相当于一个函数指针, void (*pVisit)(T &e);
	 * 据说这样的写法更具通用性
	 * VST称为函数对象TOT
	 */
	template < typename VST >
	void traverse(const VST &visit)
	{
		for (int i = 0; i < m_size; i++)
		{
			visit(m_elem[i]);
		}
	}

    /*有序向量判断是否有序*/
    int disordered()
    {
        int count = 0;

        for (int i = 1; i < m_size; i++)
        {
            if (m_elem[i - 1] > m_elem[i])
            {
                count++;
            }
        }

        return count;
    }

    /*有序向量的去重*/
    int uniquify()
    {
        /*采用传说中的双指针法*/
        Rank i = 0;
        Rank j = 0;
        
        /*以i下标的为基点,向后找,因为是有序的,我们只需要保留i的元素,后面一直找找到与v[i]不一样为止,这时候更新i*/
        while (++j < m_size)/*从第1个元素开始*/
        {
            if (m_elem[i] != m_elem[j])
            {
                m_elem[++i] = m_elem[j];
            }
        }
        m_size = ++i;
        /*删掉元素后看看要不要缩容*/
        shrink();

        return j - i;
    }

	/*有序向量的查找*/
    Rank search(T const &e, Rank lo, Rank hi)
    {
	    Rank ret = -1;
	    /*0 <= lo < hi <= size*/
	    if (lo >= 0 && hi > lo && hi <= m_size)
	    {
		    ret = biSearch(m_elem, e, lo, hi);	
		    //ret = fibSearch(m_elem, e, lo, hi);	
	    }
	    else
	    {
		    THROW_EXCEPTION(IndexOutOfBoundException, "searching element from a ordered vertor with invalid index ...");	
	    }
	    return ret;
    }

    /*向量的排序*/
    void sort(void)
    {
		if (m_size > 0)
			switch (rand() % 4)
			{
			
				case 0:
					seletionSort(0, m_size);
					break;
				case 1:
					bubbleSort_V2(m_elem, 0, m_size);
					break;
				case 2:
					insertionSort(m_elem, 0, m_size);
					break;
				default:
					mergeSort(m_elem, 0, m_size);	/*bubbleSort_V2*/
					break;
			}
			//insertionSort(m_elem, 0, m_size);
    }

    ~Vector() { delete [] m_elem; }

protected:
	int m_size;/*向量有效元素个数,即向量的规模*/
	int m_capacity;/*向量的容量*/
	T *m_elem;/*向量存储空间,实际是个动态申请的数组*/

	/*交换两个数据元素*/
	static void Swap(T &a, T &b)
	{
		T c(a);
		a = b;
		b = c;
	}

	/*供内部构造函数使用,先申请内部空间,然后将入参数组v的[lo, hi)元素拷贝到本向量的内部空间中*/
	void copyFrom(Rank lo, Rank hi, const T * const v)
	{
		m_elem = new T[m_capacity = ((hi - lo) << 1)];
		if (NULL != m_elem)
		{
			m_size = 0;
			while (lo < hi)
			{
				m_elem[m_size++] = v[lo++];	
			}
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for inner space ...");
		}
	}
	
	/*内部使用的扩容算法,当容量已满时将容量扩大到原来的一倍,使得扩容操作的均摊复杂度为O(1)*/
	void expand(void)
	{
		if (m_size < m_capacity)	/*还不需要扩容*/
			return;
		
		//cout << "we need to expand our vector capacity ..." << endl;
		if (m_capacity < DEFAULT_VECTOR_CAPACITY)
			m_capacity = DEFAULT_VECTOR_CAPACITY; /*从default容量开始*/

		T *old_elem = m_elem;
		m_elem = new T[m_capacity <<= 1];
		if (NULL == m_elem)
		{
			m_elem = old_elem;
			THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for bigger capacity space ...");
		}
		for (int i = 0; i < m_size; i++)
			m_elem[i] = old_elem[i];

		delete [] old_elem;
	}

	/*内部使用的缩容算法*/
	void shrink()
	{
		/*如果需要缩容,就将capacity减少为原来的1/2*/
		if (m_capacity < DEFAULT_VECTOR_CAPACITY << 1) return; /*保证缩容后的capacity依然大于等于default*/

		/*
		 * 计算一下装填因子,装填因子要大于25%, size / capacity > 1 /4 ==> size * 4 > capacity
		 * 在这种情况下size还是满足要求的,就不需要减少capacity的值,也就是不需要缩容
		 * 当size继续减少(也就是外部继续执行remove操作),就需要调整capacity的值来增大装填因子
		 * 否则向量的空间利用率就太低了
		 */
		if (m_size << 2 > m_capacity) return;	/*这时候装填因子是满足要求的(大于25%),先不进行缩容*/
		
		//cout << "begin shrinking ..." << endl;

		T *old_elem = m_elem;

		m_elem = new T[m_capacity >>= 1];/*减少一半容量*/
		if (NULL == m_elem)
		{
			m_elem = old_elem;
			THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new shrinking vector ...");
		}

		for (int i = 0; i < m_size; i++)
			m_elem[i] = old_elem[i];

		delete [] old_elem;
	}
	
	/*有序向量的二分查找,返回值为向量中最后一个不大于入参e的元素的秩*/
	/*二分查找的最终版本*/
	Rank biSearch(T *v, const T &e, Rank lo, Rank hi)
	{
		while (lo < hi)
		{
			Rank mi = (lo + hi) >> 1;
			(e < v[mi]) ? hi = mi : lo = mi + 1;
		}
		/*
		 *	每次循环,都必然能保证v[0, lo)都满足小于等于e的要求
		 *	[hi, n)的元素都满足大于e的要求,换而言之,v[hi]总是大于e的最小者
		 *	单看前面第一条结论,v[lo - 1]就是向量[0, lo)中最后一个小于等于e的元素
		 *	如果向量中存在等于e的元素(一个或多个都ok),这些元素的集合中必然有一个是v[lo - 1],且最大的秩为lo - 1
		 */

		/*循环退出时,hi == lo, v[lo]的值必定大于e(结论2),所以v[lo - 1]必定小于等于e,而且必然是整个有序向量中最后一个小于等于e的元素*/
		/*因此,本算法能保证,如果向量中确实有e这个元素,返回值必然是所有值为e的元素集合中秩最大的值*/
		return --lo;
	}
	
	/*二分查找版本一,在有序向量的[lo, hi)前闭后开区间中查找元素e*/
	/*必然返回查找到的元素的秩,如果有多个,则无法保证返回的是最大的,如果查不到,返回-1*/
	Rank biSearchV1(T *v, const T &e, Rank lo, Rank hi)
	{
		/*
		 *	思路:选中点mi,然后比较e和v[mi]的大小
		 *	如果e < v[mi], e一定在v的 [lo, mi)前闭后开区间中
		 *	如果 e > v[mi], 则 e一定在 (mi, hi) 前后都是开区间中,或者一定在[mi + 1, hi)前闭后开区间中,转换成这种形式有利于继续递归
		 *	否则的话 e一定等于v[mi],直接返回mi即可
		 */
		while (lo < hi)
		{
			Rank mi = (lo + hi) >> 1;
			if (e < v[mi]) hi = mi; /*[lo, mi)*/
			else if (v[mi] < e) lo = mi + 1;/*[mi + 1, hi)*/
			else return mi;
		}
		/*到这就是找不到*/
		return -1;
	}

	/*二分查找版本2*/
	/*必然返回查找到的元素的秩,如果有多个,则无法保证返回的是最大的,如果查不到,返回-1*/
	Rank biSearchV2(T *v, const T &e, Rank lo, Rank hi)
	{
		while (hi - lo > 1) /*待处理元素个数要大于1,否则按以下流程会死循环*/
		{
			/*比v1的优化点在于每次循环只进行一次比较,不管怎样都只划分两个区间*/
			Rank mi = (lo + hi) >> 1;
			(e < v[mi]) ? hi = mi : lo = mi; /*v[lo, mi)一定小于e, v[mi, hi)一定大于等于e*/
		}
		/*到这里hi = lo + 1*/

		return (e == v[lo]) ? lo : -1;
	}

	/*斐波那契查找,基于二分查找的版本一改良而来,与二分查找的区别在于mi的取值不一样*/
	/*必然返回查找到的元素的秩,如果有多个,则无法保证返回的是最大的,如果查不到,返回-1*/
	Rank fibSearch(T *v, const T &e, Rank lo, Rank hi)
	{
		Fib fib(hi - lo);

		while (hi > lo)
		{
			while (hi - lo < fib.get()) fib.prev();
			Rank mi = lo + fib.get() - 1;

			if (e < v[mi]) hi = mi; /*[lo, mi)*/
			else if (v[mi] < e) lo = mi + 1;/*[mi + 1, hi)*/
			else return mi;
		}	
		return -1;
	}
	
	/*扫描交换算法版本1,供起泡排序算法调用,完成一次扫描交换*/
	bool bubble(T *v, Rank lo, Rank hi)
	{
		bool sorted = true;

		while (++lo < hi)
		{
			if (v[lo] < v[lo - 1])
			{
				/*逆序,交换它们,同时标志位置为false*/
				sorted = false;
				swap(v[lo], v[lo - 1]);
			}
		}

		return sorted;	
	}

	/*起泡排序版本一*/
	void bubbleSort_V1(T *v, Rank lo, Rank hi)
	{
		while (!bubble(v, lo, hi)) /*如果扫描一次后不是有序的,就继续*/
		{
			/*每一轮扫描交换后,向量中最后一个元素必然就位,因此hi可以减一,也就是减治法*/
			hi--;
		}
	}

	/*扫描交换的版本二*/
	Rank bubble_V2(T *v, Rank lo, Rank hi)
	{
		Rank last = lo;

		while (++lo < hi)
		{
			if (v[lo] < v[lo - 1])
			{
				/*如果找到一对逆序对,交换它们,然后更新一下last*/
				swap(v[lo], v[lo - 1]);
				last = lo;
			}
		}	

		return last;		
	}
	
	/*起泡排序算法入口,版本2*/
	void bubbleSort_V2(T *v, Rank lo, Rank hi)
	{
		while (lo < hi)
		{
			hi = bubble_V2(v, lo, hi);
		}	
	}

	void merge(T *v, T *helper, Rank lo, Rank mi, Rank hi)
	{
		int la = mi - lo;
		int lb = la;
		int lc = hi - mi;
		T *B = helper;;
		T *A = v + lo;
		T *C = v + mi;

		for (int i = 0; i < la; i++)
			B[i] = A[i];

		for (int i = 0, j = 0, k = 0; j < lb; )
		{
			/*这里判断B[i]跟C[j]用的是小于等于,之所以要判断等于就是为了使得整个排序是稳定的*/
			A[i++] = ((k >= lc) || (B[j] <= C[k])) ? B[j++] : C[k++];
		}
	}
	
	void mergeSort(T *v, T *helper,  Rank lo, Rank hi)
	{
		/*
		 * 归并排序的思路,先不停的二分向量,直到每个子向量的元素个数为1,也就是递归基
		 * 这时候依次的对左右两侧向量进行二路归并
		 */
		/*首先处理递归基,也就是只有一个元素的情况,因为这时候向量必然是有序的*/
		if (hi - lo < 2)
			return;
		Rank mi = lo + ((hi - lo) >> 1);/*取中点,这样写可以避免溢出*/

		mergeSort(v, lo, mi);/*对[lo, hi)的子向量递归进行归并排序*/
		mergeSort(v, mi, hi);
		/*以上两步返回后, v[lo, mi)是有序的, v[mi, hi)有序的,然后再做二路归并即可以使得整个区间都是有序的*/
		/*流程上还能再优化一下,因为mi的左右两侧都是有序的,因此我们判断一下mi - 1跟mi的元素之间的关系,如果v[mi - 1] < v[mi],就没有必要做二路归并了*/
		if (v[mi - 1] > v[mi])
			merge(v, helper, lo, mi, hi);
	}

	void mergeSort(T *v, Rank lo, Rank hi)
	{
		T *helper = new T[hi - lo];

		mergeSort(v, helper, lo, hi);

		delete [] helper;	
	}

	void insertionSort(T *v, Rank lo, Rank hi)
	{
		/*平凡情况,元素个数小于2必然有序*/
		if (hi - lo < 2)
			return;	
		
		for (int i = 1; i < hi - lo; i++)
		{
			Rank pos = search(v[i], 0, i);	/*在前面一段向量中找,定位到最后一个不大于当前元素的秩,然后插入到这个元素的后面即可*/
			//cout << "pos = " << pos << endl;
			//cout << "v[pos] = " << v[pos] << endl;
			pos++;	/*++才是最终要插入的位置*/
			if (pos != i)
			{
				T tmp = v[i];
				for (int j = i; j > pos; j--)	/*一贯的套路,从后向前拷不会覆盖元素*/
				{
					v[j] = v[j - 1];
					//cout << "v[" << j << "] = v[" << j - 1 << "] ..." << endl;
				}
				v[pos] = tmp;
			}
		}
	}

	Rank max(Rank lo, Rank hi)
	{
		/*
		 * max函数是被选择排序接口调用,内部调用不再检查参数
		 * 要注意这里的语义是要在[lo, hi]左右都是闭区间的范围上找最大的元素,如果有多个重复的则返回秩最大的
		 */
		Rank ret = hi;	/*返回值初始化为查找区间最后的元素*/

		/*查找最大值的流程这样实现即可:
		 * 先初始化最大值为hi,然后在[lo, hi)注意这里是前闭后开的范围的区间上找,而且是从后向前,
		 * 如果发现存在元素值比指示器对应的元素大(一定是大于,否则当有多个最大元素时不能保证返回秩最大的),则更新指示器即可
		 */
		for (int i = hi - 1; i >= lo; i--)
		{
			if (m_elem[i] > m_elem[ret])
				ret = i;	
		}

		return ret;
	}

	void seletionSort(Rank lo, Rank hi)
	{
		/*
		 * 选择排序思想,将排序空间划分为两段,第一段是无序的,第二段是有序的,
		 * 借助迭代,每次从第一段中找到最大的插入到第二段中,迭代完成,自然就是有序的
		 */

		/*0 <= lo < hi <= size*/
		if (lo >= 0 && lo < hi && hi <= m_size)
		{
			while (lo < hi--)
			{
				Rank mx = max(lo, hi);	/*从[lo, hi]前闭后闭区间上找,因为循环判断中已经在hi减一了*/
				if (hi != mx)
					Swap(m_elem[mx], m_elem[hi]);	/*这里有点巧妙,本来是插入操作的,每次迭代插入位置就是hi这个位置,由于向量的存储空间是连续的内存,因此采用了swap的方式代替插入*/
			}	
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundException, "seletion-sort with invalid index ...");
		}
	}

private:
	enum { DEFAULT_VECTOR_CAPACITY = 5 };
};



}








#endif
