#pragma once
#ifndef __BINNODE_H__
#define __BINNODE_H_

#include <cstdlib>

#include "Object.h"
#include "Exception.h"
#include "Stack.h"

namespace MyLib
{

#define BinNodePosi(T) BinNode<T> *
/*获取以p为根节点的子树的高度*/
#define stature(p) ((p) ? (p)->m_height : -1)

template < typename T >
class BinNode : public Object
{
public:
    T m_data;	/*实际数据*/
    BinNodePosi(T) m_parent;    /*指向当前节点父亲的引用*/
    BinNodePosi(T) m_leftchild; /*指向左孩子节点的引用*/
    BinNodePosi(T) m_rightchild; /*指向右孩子节点的引用*/
    int m_height;   /*当前节点在树中的高度*/

    BinNode();  /*无参构造函数*/
    BinNode(const T &data, BinNodePosi(T) parent = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int height = 0);

    /*以下定义二叉树节点的操作接口*/
    int size(void) const;   /*统计当前节点后代总数,即以当前节点为根节点的子树的规模*/
    BinNodePosi(T) insertAsLeftChild(const T &e);   /*作为当前节点的左孩子插入*/
    BinNodePosi(T) insertAsRightChild(const T &e);   /*作为当前节点的右孩子插入*/

    BinNodePosi(T) succ(void);  /*在中序遍历意义下,当前节点的直接后继*/

    template < typename VST >
    void traverLevel(const VST &visit);   /*子树层次遍历*/
    
    template < typename VST >
    void traverPre(const VST &visit);   /*子树先序遍历*/

    template < typename VST >
    void traverIn(const VST &visit);   /*子树中序遍历*/

    template < typename VST >
    void traverPost(const VST &visit);   /*子树后序遍历*/

	virtual ~BinNode();

private:
	/*所有成员变量/函数定义为public,后续可根据工程实际需要修改*/
    template < typename VST >
    void travPre_recursive(BinNodePosi(T) x, const VST &visit);

    /*先序遍历迭代实现,版本1*/
    template < typename VST >
    void travPre_iter_V1(BinNodePosi(T) x, const VST &visit);
};

template < typename T >
BinNode<T>::BinNode()
    : m_data(),
    m_parent(NULL),
    m_leftchild(NULL),
    m_rightchild(NULL),
    m_height(0)
{}

template < typename T >
BinNode<T>::BinNode(const T &data, BinNodePosi(T) parent/* = NULL*/, BinNodePosi(T) lc/* = NULL*/, BinNodePosi(T) rc/* = NULL*/, int height/* = 0*/)
    : m_data(data),
    m_parent(parent),
    m_leftchild(lc),
    m_rightchild(rc),
    m_height(height)
{
}

template < typename T >
BinNodePosi(T) BinNode<T>::insertAsLeftChild(const T &e)
{
    BinNodePosi(T) ret = NULL;

    /*当前节点的左孩子必须不存在才能插入成功*/
    if (NULL == this->m_leftchild)
    {
        ret = new BinNode(e, this);
        if (NULL != ret)
            this->m_leftchild = ret;
        else
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new left child ...");
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "Already have left child ...");
    }

    /*在常数时间内完成, O(1)*/
    return ret;
}

template < typename T >
BinNodePosi(T) BinNode<T>::insertAsRightChild(const T &e)
{
    BinNodePosi(T) ret = NULL;

    /*当前节点的右孩子必须不存在才能插入成功*/
    if (NULL == this->m_rightchild)
    {
        ret = new BinNode(e, this);
        if (NULL != ret)
            this->m_rightchild = ret;
        else
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new right child ...");
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "Already have right child ...");
    }

    /*在常数时间内完成, O(1)*/

    return ret;
}

template < typename T >
int BinNode<T>::size(void) const
{
    int ret = 1;    /*自己肯定算一个*/

    /*分别递归求左孩子和右孩子的规模,加起来即可*/
    if (NULL != this->m_leftchild)
        ret += this->m_leftchild->size();
    if (NULL != this->m_rightchild)
        ret += this->m_rightchild->size();

    /*时间复杂度与以当前节点为根的子树规模成正比,即O(size)*/

    return ret;
}

template < typename T >
template < typename VST >
void BinNode<T>::traverPre(const VST &visit)
{
    switch (rand() % 2)
    {
        case 1: { travPre_recursive(this, visit); break; }
        case 2: { travPre_iter_V1(this, visit); break; }
    }
}

template < typename T >
template < typename VST >
void BinNode<T>::travPre_recursive(BinNodePosi(T) x, const VST &visit)
{
    if (NULL == x)
        return;

    visit(x->m_data);
    travPre_recursive(x->m_leftchild, visit);
    travPre_recursive(x->m_rightchild, visit);
}   //T(n) = O(1) + T(a) + T(n -a - 1) = O(n) 

template < typename T >
template < typename VST >
void BinNode<T>::travPre_iter_V1(BinNodePosi(T) x, const VST &visit)
{
    Stack<BinNodePosi(T)> s; /*辅助栈*/

    if (NULL != x)
        s.push(x);

    while (!s.empty())
    {
        /*每次我们访问栈顶元素,并将其弹出*/
        x = s.pop();
        visit(x->m_data);

        /*右顾左盼:先看看当前节点x有无右孩子,如果有,将其入栈*/
        if (HasRChild(*x))
            s.push(x->m_rightchild);
        /*接着再来看看当前节点有无左孩子,如果有,将其入栈*/
        /*由于栈的后进先出特性,因此先访问x的右孩子,在访问x的左孩子,最终使得我们遍历的顺序符合先序遍历的要求*/
        if (HasLChild(*x))
            s.push(x->m_leftchild);
    }
}


template < typename T >
BinNode<T>::~BinNode()
{
}

/*下面定义一些宏,提供一些对二叉树节点相关的便捷操作接口*/
#define IsRoot(x) (NULL == (x).m_parent)
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).m_parent->m_leftchild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).m_parent->m_rightchild))

/*判断节点x有无父亲节点,根节点就没有,其他都有*/
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) (NULL != (x).m_leftchild)
#define HasRChild(x) (NULL != (x).m_rightchild)
/*至少拥有一个孩子*/
#define HasChild(x) (HasLChild(x) || HasRChild(x))
/*同时拥有两个孩子*/
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
/*是否是叶节点,即孩子数是否为0*/
#define IsLeaf(x) (!HasChild(x))

/*与BinNode具有特定关系的节点以及指针*/

/*获取兄弟节点,即如果是左孩子就返回对应父亲的右孩子,反之则返回左孩子*/
/*注意这里的实现并没有判断其是否就是根节点*/
#define sibling(p) ( \
    IsLChild(*(p)) ? \
        (p)->m_parent->m_rightchild : \
        (p)->m_parent->m_leftchild \
)

/*获取节点位置p的叔叔,也就是它的父亲的兄弟*/
#define uncle(p) (  \
    IsLChild((p)->m_parent) ?   \
        (p)->m_parent->m_parent->m_rightchild : \
        (p)->m_parent->m_parent->m_leftchild    \
)

/*获取当前节点的父亲指向它的节点位置,即如果当前节点是左孩子的话,返回其父亲的m_leftchild,反之返回m_rightchild*/
/*如果当前节点本身是根节点的话,返回m_root节点,即它本身*/
#define FromParentTo(x) (   \
    IsRoot(x) ? m_root : (  \
        IsLChild(x) ? (x).m_parent->m_leftchild :   \
            (x).m_parent->m_rightchild  \
    )   \
)



}

#endif
