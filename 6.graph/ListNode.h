#pragma once

#ifndef __LISTNODE_H__
#define __LISTNODE_H__

#include "Object.h"
#include "Exception.h"

namespace MyLib

{
typedef int Rank;   /*列表的秩*/
#define Posi(T) ListNode<T> * /*定义列表节点位置*/

template < typename T >
struct ListNode : public Object
{
    T m_data; /*列表节点实际存放的数据*/
    Posi(T) m_pred;   /*当前节点的前驱predecessor*/
    Posi(T) m_succ;   /*当前节点的后继successor*/

    /*构造函数定义*/
    //ListNode() {}
    ListNode(const T &e, Posi(T) p = NULL, Posi(T) s = NULL);

    /*列表节点的ADT接口声明*/
    /*插入前驱节点,存入被引用对象e,返回新节点位置*/
    Posi(T) insertAsPred(const T & e);

    /*插入后继节点,存入被引用对象e,返回新节点位置*/
    Posi(T) insertAsSucc(const T &e);

    /*返回前驱节点位置*/
    Posi(T) pred();

    /*返回当前节点的后继节点位置*/
    Posi(T) succ();

    /*当前节点存储的数据*/
    T &data();

	/*TODO:实现工厂模式?*/

    ~ListNode();
};

template < typename T >
ListNode<T>::ListNode(const T &e, Posi(T) p, Posi(T) s)
    : m_data(e),
    m_pred(p),
    m_succ(s)
{
}

template < typename T >
Posi(T) ListNode<T>::insertAsPred(const T &e)
{
    Posi(T) ret = new ListNode<T>(e, this->m_pred, this);   /*新插入的节点,它的前驱是当前节点的前驱,后继是当前节点*/

    if (NULL != ret)
    {
        /*
        *   list中的每个有效节点(visible节点)本身实际上与4个指针有关,上面通过ListNode的构造函数修改了2个指针,还剩下两个指针需要修改
        *   新插入节点是当前节点的前驱,当前节点的前驱的后继(原来指向当前节点)修改为指向新节点
        */
        this->m_pred->m_succ = ret;
        this->m_pred = ret;

    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new list node ...");
    }
    
    /*最后将新节点的位置返回*/
    return ret;
}

template < typename T >
Posi(T) ListNode<T>::insertAsSucc(const T &e)
{
    Posi(T) ret = new ListNode<T>(e, this, this->m_succ);   /*新节点插入到当前节点的后继位置,它的前驱是当前节点,后继是当前节点的后继*/

    if (NULL != ret)
    {
		//cout << "new list node, addr = " << ret << endl;
        /*同样的修改剩余的两个指针*/
        this->m_succ->m_pred = ret;
        this->m_succ = ret;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new successor node ...");
    }
    
    /*最后将新节点的位置返回*/
    return ret;
}

template < typename T >
T &ListNode<T>::data()
{
	return m_data;
}

template < typename T >
Posi(T) ListNode<T>::pred()
{
	return m_pred;
}

template < typename T >
Posi(T) ListNode<T>::succ()
{
	return m_succ;
}

template < typename T >
ListNode<T>::~ListNode()
{
    //cout << "~ListNode()" << endl;
}


}

#endif
