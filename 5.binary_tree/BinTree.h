#pragma once
#ifndef __BINTREE_H__
#define __BINTREE_H__

#include <algorithm>
#include "BinNode.h"

namespace MyLib
{

template < typename T >
class BinTree : public Object
{
public:
    BinTree();

    /*获取二叉树的规模*/
    int size(void) const { return m_size; }
    /*判空操作*/
    bool empty(void) const { return (NULL == m_root); }

    /*获取二叉树的根节点*/
    BinNodePosi(T) root(void) const { return m_root; }

    /*删除以x为根节点的子树,并返回删除的子树规模*/

    /*作为根节点插入空的二叉树*/
    BinNodePosi(T) insertAsRoot(const T &e);
    /*将data作为x的左孩子插入,并返回新插入节点的位置*/
    BinNodePosi(T) insertAsLeftChild(BinNodePosi(T) x, const T &data);
    /*将data作为x的右孩子插入,并返回新插入节点的位置*/
    BinNodePosi(T) insertAsRightChild(BinNodePosi(T) x, const T &data);

    /*子树接入,将子树S当做节点x的左子树接入,同时将S这个指针置空A*/
    BinNodePosi(T) attachAsLeftChild(BinNodePosi(T) x, BinTree<T> *&S);
    BinNodePosi(T) attachAsRightChild(BinNodePosi(T) x, BinTree<T> *&S);

    template < typename VST >
    void travPre(const VST &visit);

     ~BinTree();

protected:
    /*约定当m_root为空时m_size为0*/
    int m_size; /*记录当前二叉树规模*/
    BinNodePosi(T) m_root;  /*二叉树的根节点*/

    virtual int updateHeight(BinNodePosi(T) x); /*更新节点x的高度*/
    void updateHeightAbove(BinNodePosi(T) x);   /*更新节点x及其祖先的高度*/
};

template < typename T >
BinTree<T>::BinTree()
    : m_size(0),
    m_root(NULL)
{
}

template < typename T >
BinNodePosi(T) BinTree<T>::insertAsRoot(const T &e)
{
    /*检查是否是空树*/
    if (NULL == m_root)
    {
        m_root = new BinNode<T>(e);
        if (NULL == m_root)
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for root node ...");

        m_size = 1;
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "Could not insert root node to a non-empty tree ...");
    }
    
    return m_root;
}


/*更新树高度接口,每次当一个树节点的孩子或它的后代们发生变化啊时,它的高度都有可能发生变化,注意是有可能,因为发生变化的子树的高度并不是当前节点所有子树中高度最高的,这种情况下节点高度就不会发生变化*/
/*此外,节点对其后代发生变化可能是不知情的,所以更新高度只能是从发生变化的节点开始通过m_parent引用依次的更新它的祖先们的高度, 这也就是updateHeightAbove接口存在的意义*/
template < typename T >
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
    x->m_height = 1 + ::std::max(stature(x->m_leftchild), stature(x->m_rightchild));

    /*O(1)*/
    return x->m_height;
}

template < typename T >
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
    while (NULL != x)
    {
        /*从当前节点开始,依次更新高度*/
        updateHeight(x);
        x = x->m_parent;
    }

    /*时间复杂度应正比于节点x的深度, O(depth(x))*/
}

template < typename T >
BinNodePosi(T) BinTree<T>::insertAsLeftChild(BinNodePosi(T) x, const T &data)
{
    x->insertAsLeftChild(data); /*此函数中能保证x的左孩子为空,否则会抛出异常*/
    updateHeightAbove(x);
    m_size++;   /*树的规模记得更新,同时要注意异常安全*/

    return x->m_leftchild;
}


template < typename T >
BinNodePosi(T) BinTree<T>::insertAsRightChild(BinNodePosi(T) x, const T &data)
{
    x->insertAsRightChild(data); /*检查有无右孩子在BinNode函数中实现*/
    updateHeightAbove(x);
    m_size++;   /*树的规模记得更新,同时要注意异常安全*/

    return x->m_leftchild;
}

template < typename T >
BinNodePosi(T) BinTree<T>::attachAsLeftChild(BinNodePosi(T) x, BinTree<T> *&subtree)
{
    if (!HasLChild(*x))
    {
        if (NULL != subtree)
        {
            x->m_leftchild = subtree->m_root;
            if (NULL != x->m_leftchild)
                x->m_leftchild->m_parent = x;   /*父子关系构建一下*/
            this->m_size += subtree->m_size;

            updateHeightAbove(x);   /*更新高度信息*/
            /*subtree本身维护的树已经转到x的左孩子处了,清理一下*/
            subtree->m_root = NULL;
            subtree->m_size = 0;
            delete subtree;
            subtree = NULL;
        }
        else
        {
            /*如果subtree对象是空的话就啥都不用做了,或者抛异常?*/
        }
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "tree node x's left subtree is not empty, could not attach subtree ...");
    }
    return x;
}

template < typename T >
BinNodePosi(T) BinTree<T>::attachAsRightChild(BinNodePosi(T) x, BinTree<T> *&subtree)
{
    if (!HasRChild(*x))
    {
        if (NULL != subtree)
        {
            x->m_rightchild = subtree->m_root;
            if (NULL != x->m_rightchild)
                x->m_rightchild->m_parent = x;   /*父子关系构建一下*/
            this->m_size += subtree->m_size;

            updateHeightAbove(x);   /*更新高度信息*/
            /*subtree本身维护的树已经转到x的左孩子处了,清理一下*/
            subtree->m_root = NULL;
            subtree->m_size = 0;
            delete subtree;
            subtree = NULL;
        }
        else
        {
            /*如果subtree对象是空的话就啥都不用做了,或者抛异常?*/
        }
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "tree node x's right subtree is not empty, could not attach subtree ...");
    }
    return x;
}

template < typename T >
template < typename VST >
void BinTree<T>::travPre(const VST &visit)
{
    /*这里需要判空,以防编译器优化,因为这里->即解引用,编译器有可能认为m_root必然不是一个空指针,也不是野指针*/
    if (NULL != m_root)
        m_root->traverPre(visit);
}

template < typename T >
BinTree<T>::~BinTree()
{
}


}

#endif
