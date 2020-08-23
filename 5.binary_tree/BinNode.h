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
    BinNodePosi(T) pred(void);  /*在中序遍历意义下,当前节点的直接前驱*/

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

    /*先序遍历迭代实现,版本2*/
    /*用于先序遍历迭代版本2,实现沿x的左侧链一路下行访问,同时将每个左孩子的兄弟入栈*/
    template < typename VST >
    void visitAlongLeftBranch(BinNodePosi(T) x, const VST &visit, Stack<BinNodePosi(T)> &S);
    
    template < typename VST >
    void travPre_iter_V2(BinNodePosi(T) x, const VST &visit);

    /*中序遍历递归实现*/
    template < typename VST >
    void traverIn_recursion(BinNodePosi(T) x, const VST &visit);

    template < typename VST >
    void traverIn_iter_V1(BinNodePosi(T) x, const VST &visit);

    /*子功能函数,用于中序遍历时,实现从根节点x开始,沿着其左侧链(藤曼)一路下行,同时将遇到的节点统统入栈,直到遇到一个节点,它的左子树为空,迭代结束*/
    void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)> &s);

    template < typename VST >
    void traverIn_iter_V2(BinNodePosi(T) x, const VST &visit);

    /*中序遍历迭代版本3,借助标志位取消掉辅助栈的使用,使得空间复杂度从O(n)变为O(1)*/
    template <typename VST>
    void traverIn_iter_V3(BinNodePosi(T) x, const VST &visit);
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
    switch (rand() % 3)
    {
        case 1: { travPre_recursive(this, visit); break; }
        case 2: { travPre_iter_V1(this, visit); break; }
        case 3: { travPre_iter_V2(this, visit); break; }
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
template < typename VST >
void BinNode<T>::visitAlongLeftBranch(BinNodePosi(T) x, const VST &visit, Stack<BinNodePosi(T)> &s)
{
    /*本函数负责实现二叉树中序遍历中沿节点x的左侧链不断下行的逻辑*/
    /*算法复杂度分摊O(1)*/
    while (NULL != x)
    {
        /*先访问x*/
        visit(x->m_data);
        /*x的右孩子入栈*/
        if (NULL != x->m_rightchild)
            s.push(x->m_rightchild);

        /*将控制权交给x的左孩子*/
        x = x->m_leftchild;
    }
}

template < typename T >
template < typename VST>
void BinNode<T>::travPre_iter_V2(BinNodePosi(T) x, const VST &visit)
{
    bool loop = true;
    Stack<BinNodePosi(T)> s;

    while (loop)
    {
        visitAlongLeftBranch(x, visit, s);
        if (s.size() <= 0)
            loop = false;
        else
            x = s.pop();
    }
    /*pop = push = visit = O(n) = 分摊O(1)*/
}

template < typename T >
template < typename VST >
void BinNode<T>::traverIn(const VST &visit)
{
    srand(time(NULL));
    switch (rand() % 4)
    {
        case 0 : { traverIn_recursion(this, visit); break; }
        case 1 : { traverIn_iter_V1(this, visit); break; }
        case 2 : { traverIn_iter_V2(this, visit); break; }
        case 3 : { traverIn_iter_V3(this, visit); break; }
    }

}

template < typename T >
template < typename VST >
void BinNode<T>::traverIn_recursion(BinNodePosi(T) x, const VST &visit)
{
    /*递归基:当前节点是空*/
    if (NULL == x) return;

    /*先访问左子树*/
    traverIn_recursion(x->m_leftchild, visit);
    /*然后访问当前子树根节点*/
    visit(x->m_data);
    /*再是根节点的右子树*/
    traverIn_recursion(x->m_rightchild, visit);
}

template < typename T >
void BinNode<T>::goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)> &s)
{
    while (x)
    {
        s.push(x);
        x = x->m_leftchild;
    }
}

template < typename T >
template < typename VST >
void BinNode<T>::traverIn_iter_V1(BinNodePosi(T) x, const VST &visit)
{
    Stack<BinNodePosi(T)> s;

    while (true)
    {
        goAlongLeftBranch(x, s);
        /*一旦栈变空,说明所有节点都已经遍历完,退出迭代*/
        if (s.empty())
            break;
        x = s.pop();    /*x就是那一个左子树为空的节点(或者更准确的说应当是x的左子树已经遍历完了)*/
        visit(x->m_data);   /*那么接下来访问权就移至x*/
        x = x->m_rightchild;    /*转而访问x的右子树*/
    }
}

template < typename T >
template < typename VST >
void BinNode<T>::traverIn_iter_V2(BinNodePosi(T) x, const VST &visit)
{
    Stack<BinNodePosi(T)> s;

    /*中序遍历迭代版本2就是将goAlongLeftBranch函数去掉了*/

    while (true)
    {
        if (NULL != x)  /*相当于goAlongLeftBranch*/
        {
            /*如果左孩子不为空,就将当前根节点入栈,并将控制权转交至左孩子*/
            s.push(x);
            x = x->m_leftchild;
        }
        else if (!s.empty())  /*否则看看栈中有无元素,有的话就已经到达转折点*/
        {
            x = s.pop();
            visit(x->m_data);
            /*转至右子树这个局部*/
            x = x->m_rightchild;
        }
        else
        {
            break;  /*如果栈为空,迭代结束*/
        }
    }
}

template < typename T >
template < typename VST >
void BinNode<T>::traverIn_iter_V3(BinNodePosi(T) x, const VST &visit)
{
    bool backtrace = false;

    ::std::cout << "inorder traverse : iterator version 3 ..." << ::std::endl;

    while (true)
    {
        /*要注意回溯完不能继续沿左侧链下行,而应当进入else分支即访问当前节点*/
        if ((NULL != x->m_leftchild) && (!backtrace))
        {
            x = x->m_leftchild; /*沿左侧链一直下行*/
        }
        else
        {
            /*先访问当前节点*/
            visit(x->m_data);
            if (HasRChild(*x))
            {
                /*转入右子树*/
                x = x->m_rightchild;
                backtrace = false;  /*进入另一个局部,回溯标志位应当清空*/
            }
            else
            {
                backtrace = true;
                /*回溯*/
                x = x->succ();  /*需要回溯,借助succ接口获取下一个访问的节点*/
                if (NULL == x)
                    break;  /*直接后继为空,说明遍历已经完成*/
            }
        }
    }
}


template < typename T >
BinNodePosi(T) BinNode<T>::succ(void)
{
    BinNodePosi(T) ret = this;

    if (NULL != ret->m_rightchild)
    {
        /*如果右子树存在,则转入右子树中,自成一局部,然后继续沿着左侧链(藤曼)一直下行,直到找到一个没有左孩子的节点,这个节点就是我们下一次要访问的节点*/
        ret = ret->m_rightchild;
        while (HasLChild(*ret))
            ret = ret->m_leftchild;
        /*此分支的时间复杂度应当为this->m_rightchild节点所在树中的高度*/
    }
    else
    {
        /*
        *   否则,直接后继就是"将当前节点包含于其左子树中的最低祖先"
        *   因为中序遍历中一个局部的根节点必然先于它的右子树中的所有成员被访问,而左子树又必然先于根节点被访问
        */
        while (IsRChild(*ret))
            ret = ret->m_parent;    /*沿着右侧链上行,while循环结束时ret必然是我们所求的直接后继的右孩子*/

        ret = ret->m_parent;    /*直接后继的右孩子的父亲即为所求,注意有可能直接后继是不存在的,这时候其实while循环结束后ret == m_root, m_root的父亲约定为空,意思就是遍历已经结束*/
    }

    return ret;
}

template < typename T >
BinNodePosi(T) BinNode<T>::pred(void)
{
    BinNodePosi(T) ret = this;

    if (NULL != ret->m_leftchild)
    {
        ret = ret->m_leftchild;
        /*沿着this节点的左子树右侧链下行,找到第一个没有没有右子树的节点,中序遍历时此处将发生回溯,在访问完这个节点后就回溯到this节点处,即this节点的前驱就是它*/
        while (HasRChild(*ret))
            ret = ret->m_rightchild;
    }
    else
    {
        /*与求后继的刚好取反,直接前驱应当是包含当前节点在其右子树中的最低祖先*/
        while (IsLChild(*ret))
            ret = ret->m_parent;

        ret = ret->m_parent;
    }

    return ret;
}

template < typename T >
BinNode<T>::~BinNode()
{
}

}

#endif
