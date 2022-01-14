#pragma once 

#ifndef __BINNODE_MACRO_H__
#define __BINNODE_MACRO_H__

namespace MyLib
{

#define BinNodePosi(T) BinNode<T> *
/*获取以p为根节点的子树的高度,约定空树高度为-1,因为根节点高度为0*/
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

}

#endif
