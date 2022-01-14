#ifndef __BST_H__
#define __BST_H__

#include "BinTree.h"

namespace MyLib
{

template < typename T >
class BST : public BinTree<T>
{
public:
    virtual BinNodePosi(T) &search(const T &e);     /*BST的查找操作*/
    virtual BinNodePosi(T) insert(const T &e);     /*BST的插入操作*/
    virtual BinNodePosi(T) remove(const T &e);    /*BST的删除操作*/

protected:
    static BinNodePosi(T) removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot, BinNodePosi(T) &succ);

    BinNodePosi(T) m_hot;   /*记录BST::search接口最后访问的非空(除非树空)的节点位置(在整个查找过程中,m_hot始终指向当前处理节点的父亲)*/

private:
    BinNodePosi(T) &searchIn(BinNodePosi(T) &v, const T &e, BinNodePosi(T) &hot);
    BinNodePosi(T) &searchInDup(BinNodePosi(T) &v, const T &e, BinNodePosi(T) &hot, const bool go_left_branch);

};

template < typename T >
BinNodePosi(T) &BST<T>::searchIn(BinNodePosi(T) &v, const T &e, BinNodePosi(T) &hot)
{
    if (nullptr == v || e == v->m_data)
        return v;

    /*类似二分查找即可,查找元素比当前节点小则进入左子树,否则进入右子树*/

    /*每次迭代都更新hot*/
    hot = v;

    return searchIn((e < v->m_data) ? v->m_leftchild : v->m_rightchild, e, hot);
}

template < typename T >
BinNodePosi(T) &BST<T>::search(const T &e)
{
    return searchIn(this->m_root, e, m_hot = nullptr);
}

template <typename T>
BinNodePosi(T) BST<T>::insert(const T& e)
{
    BinNodePosi(T) &ret = searchInDup(this->m_root, e, m_hot = nullptr, false);

    /*
     * 实际上search接口的返回值永远指向值为e的节点处,而且是一个引用
     * 如果树中不存在值为e的节点,则search返回值应当指向空节点
     * 此时我们可以做插入操作,即插入到ret所指向的位置处即可
     * 由于暂时不支持插入重复节点,所以如果树中已经存在对应节点,则不允许插入
     */
    if (nullptr == ret)
    {
        ret = new BinNode<T>(e, m_hot); 
        if (nullptr == ret)
            THROW_EXCEPTION(NoEnoughMemoryException, "no enough memory  for new BST node ...");

        /*更新树规模*/
        this->m_size++;

        /*更新所有相关节点的高度*/
        this->updateHeightAbove(ret);
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "insert bst node failed ...");
    }

    return ret;
}

/*删除位置x所指的节点,将之返回出去,hot指向被删除节点的父亲,succ指向实际被删除节点的接替者*/
template < typename T >
BinNodePosi(T) BST<T>::removeAt(BinNodePosi(T) &x, BinNodePosi(T) &hot, BinNodePosi(T) &succ)
{
    BinNodePosi(T) ret = x;
    succ = nullptr;

    if (!HasLChild(*x))
    {
        x = x->m_rightchild;
        succ = x;

        if (nullptr != succ)
            succ->m_parent = hot;
    }
    else if(!HasRChild(*x))
    {
        x = x->m_leftchild;
        succ = x;

        if (nullptr != succ)
            succ->m_parent = hot;
    }
    else
    {
        /*
         * 待删除节点左右子树都存在的情况
         * 我们通过succ函数获取x节点的直接后继,因为是直接后继,这个直接后继必然只有0个或一个孩子,我们将直接后继的m_data与x的m_data进行交换,这时候将后继删除即可
         */
        
        succ = x->succ();
        swap(x->m_data, succ->m_data);

        /*此时我们需要将直接后继succ从树中删除*/
        ret = succ;

        /*
         * succ此时有0或1个孩子,而且必然是右孩子,我们要把succ从树中拿掉,还剩下一步就是重新构建succ的父亲与succ的右孩子之间的关系
         * 这里有两种情况,即将succ的右孩子接到succ父亲的左子树还是右子树中
         * 如果succ不是x的右孩子的话,那么succ必然是其父亲的左孩子,也就是将succ的右孩子接入succ父亲的左子树即可
         */
        
        BinNodePosi(T) parent = succ->m_parent;
        if (x != parent)
            parent->m_leftchild = succ->m_rightchild;
        else
            parent->m_rightchild = succ->m_rightchild;

        /*此时再来更新succ,succ指向实际被删除节点的接替者,也就是succ的右孩子*/
        succ = succ->m_rightchild;
        /*构建succ右孩子的父亲引用,因为succ需要被删掉*/
        hot = parent;
        if (nullptr != succ)
            succ->m_parent = hot;
    }

    ret->m_leftchild = ret->m_rightchild = nullptr;

    return ret;
}

template < typename T >
BinNodePosi(T) BST<T>::remove(const T& e)
{
    BinNodePosi(T) ret = nullptr;
    BinNodePosi(T) succ = nullptr;
    BinNodePosi(T) &toDel = searchInDup(this->m_root, e, m_hot = nullptr, true);//search(e);
    if (nullptr == toDel)
        return ret;   /*未查找到对应节点,删除失败*/

    ret = removeAt(toDel, m_hot, succ);
    this->m_size--;     /*更新规模*/
    this->updateHeightAbove(m_hot);   /*更新节点高度*/

    return ret;
}

template < typename T >
BinNodePosi(T) &BST<T>::searchInDup(BinNodePosi(T) &v, const T &e, BinNodePosi(T) &hot, const bool go_left_branch)
{
    if (nullptr == v)
        return v;

    bool found = false;
    BinNodePosi(T) &hit = v;
    BinNodePosi(T) &cur = v;

    while (cur)
    {
        ::std::cout << "..." << ::std::endl;
        if (e != cur->m_data)
        {
            hot = cur;  /*即将进入下一轮迭代,更新一下hot节点*/
            cur = (cur->m_data < e) ? cur->m_leftchild : cur->m_rightchild;
        }
        else
        {
            hit = cur;
            found = true;
            hot = cur;  /*即将进入下一轮迭代,更新一下hot节点*/
            cur = (go_left_branch) ? cur->m_leftchild : cur->m_rightchild;
        } 
    }

    if (found)
    {
        cur = hit;
        hot = hit->m_parent;
    }

    return cur;
}

}



#endif

