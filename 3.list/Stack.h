/*
 *	栈数据结构实现
 */

#include "Vector.h"

namespace MyLib
{

/*
 *	栈实际上是序列的一种特例,只不过栈只能从一侧进行插入或删除操作
 *	而另一侧(称为盲端)则不能进行操作,因此直接让Stack类作为Vector类的直接子类,
 *	在此基础上提供栈数据结构的操作接口即可
 *
 */

template < typename T >
class Stack : public Vector<T>
{
public:

	Stack() {}

	/*入栈接口*/
	void push(const T &e)
	{
		this->insert(Vector<T>::size(), e);
	}
	/*出栈*/
	T pop()
	{
		return this->remove(Vector<T>::size() - 1);
	}

	/*获取栈顶元素*/
	T &top()
	{
		return ((*this)[Vector<T>::size() - 1]);
	}
	/*获取栈当前大小*/
	int size() const
	{
		return Vector<T>::size();
	}

	/*判断栈是否为空*/
	bool empty() const
	{
		return (0 == Vector<T>::size());
	}

	~Stack() {}

};


}
