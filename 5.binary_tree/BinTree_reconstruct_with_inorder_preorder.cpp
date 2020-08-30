#include <iostream>

#include "BinTree.h"

class visit
{
public:
    void operator ()(int &data) const
    {
        cout << data << endl;
    }
};

template < typename T >
MyLib::BinNode<T> *reconstruct(MyLib::Vector<T> &preorder, int pre_start, int pre_end, MyLib::Vector<T> &inorder, int in_start, int in_end)
{
    MyLib::BinNode<T> *ret = NULL;
    int to_process = pre_end - pre_start;   /*待处理节点个数,即输入数组的有效长度*/
  
    //cout << "enter func " << __FUNCTION__ << endl;
    //cout << "prestart = " << pre_start << " pre end = " << pre_end << " in_start = " << in_start << " in end = " << in_end << endl;

    if (to_process <= 1)
    {
        /*递归基,只有先序遍历中只剩下一个节点了,这时候这个节点必然就是根节点,且没有孩子*/
        ret = new MyLib::BinNode<T> (preorder[pre_start]); 
    }
    else
    {
        int root = preorder[pre_start];
        int rank_of_in = inorder.find(in_start, in_end, root);

        //cout << "rank of in = " << rank_of_in << endl;

        ret = new MyLib::BinNode<T> (preorder[pre_start]); 
        if (-1 == rank_of_in)
        {
            delete ret;
            THROW_EXCEPTION(MyLib::InvalidOperationException, "failed to find root from inorder vector ...");
        }
        else
        {
            int size_of_left = rank_of_in - in_start;  /*左子树规模*/ 
            int size_of_right = to_process - 1 - size_of_left;  /*右子树规模*/
            //cout << "size of left = " << size_of_left << endl;
            //cout << "size of right = " << size_of_right << endl;
            /*构建左子树*/
            if (0 != size_of_left)
            {
                cout << "constructing left tree ..." << endl;
                MyLib::BinNode<T> *subtree_l = reconstruct(preorder, pre_start + 1, pre_start + size_of_left + 1, inorder, in_start, in_start + size_of_left);

                ret->m_leftchild = subtree_l;
                subtree_l->m_parent = ret;
            }
            else
            {
                cout << "left tree is NULL ..." << endl;
                ret->m_leftchild = NULL;
            }

            /*构建右子树*/
            if (0 != size_of_right)
            {
                cout << "constructing right tree ..." << endl;
                MyLib::BinNode<T> *subtree_r = reconstruct(preorder, pre_start + size_of_left + 1, pre_end, inorder, in_start + size_of_left + 1, in_end); 

                ret->m_rightchild = subtree_r;
                subtree_r->m_parent = ret;
            }
            else
            {
                cout << "right tree is NULL ..." << endl;
                ret->m_rightchild = NULL;
            }
        }
    }

    return ret;
}

/*通过先序遍历和中序遍历还原二叉树*/
template < typename T >
void bintree_reconstruction_preorder(MyLib::Vector<T> &preorder, MyLib::Vector<T> &inorder)
{
    try
    {
        MyLib::BinNode<T> *root = reconstruct(preorder, 0, preorder.size(), inorder, 0, inorder.size());
        visit visitor;
        //cout << root->m_data << endl;
        root->traverLevel(visitor);
    }
    catch (const MyLib::Exception &obj)
    {
        cout << obj.message() << endl;
        cout << obj.location() << endl;
    }
    catch (...)
    {
        cout << "..." << endl;
    }
}
