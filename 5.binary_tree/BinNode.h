#include "Object.h"

namespace MyLib
{

#define BinNodePosi(T) BinNode<T> *

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
	BinNode(T &data, BinNodePosi(T) parent);

    /*以下定义二叉树节点的操作接口*/
    int size() const;   /*统计当前节点后代总数,即以当前节点为根节点的子树的规模*/
    BinNodePosi(T) insertAsLeftChild(const T &e);   /*作为当前节点的左孩子插入*/
    BinNodePosi(T) insertAsRightChild(const T &e);   /*作为当前节点的右孩子插入*/

    BinNodePosi(T) succ(void);  /*在中序遍历意义下,当前节点的直接后继*/

    template < typename VST >
    void traverLevel(VST &visit);   /*子树层次遍历*/
    
    template < typename VST >
    void traverPre(VST &visit);   /*子树先序遍历*/
    template < typename VST >
    void traverIn(VST &visit);   /*子树中序遍历*/

    template < typename VST >
    void traverPost(VST &visit);   /*子树后序遍历*/

	~BinNode();

private:
	/*所有成员变量/函数定义为public,后续可根据工程实际需要修改*/
};


}
