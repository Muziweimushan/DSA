#include <iostream>

#include "BinTree.h"
#include "TrieTreeNode.h"
using namespace MyLib;

template < typename T >
class visit
{
public:
    void operator () (const T &data) const
    {
        ::std::cout << data << ::std::endl; 
    }
};

template < typename T >
BinNode<T> *reconstruct(Vector<T> &preorder, int pre_start, int pre_end, Vector<T> &inorder, int in_start, int in_end)
{
    BinNode<T> *ret = NULL;
    int to_process = pre_end - pre_start;   /*待处理节点个数,即输入数组的有效长度*/
    int rank_of_root = pre_start; 
    //cout << "enter func " << __FUNCTION__ << endl;
    //cout << "prestart = " << pre_start << " pre end = " << pre_end << " in_start = " << in_start << " in end = " << in_end << endl;

    if (to_process <= 1)
    {
        /*递归基,只有先序遍历中只剩下一个节点了,这时候这个节点必然就是根节点,且没有孩子*/
        ret = new BinNode<T> (preorder[rank_of_root]); 
    }
    else
    {
        int root = preorder[rank_of_root];
        int rank_of_in = inorder.find(in_start, in_end, root);

        //cout << "rank of in = " << rank_of_in << endl;

        ret = new BinNode<T> (preorder[rank_of_root]); 
        if (-1 == rank_of_in)
        {
            delete ret;
            THROW_EXCEPTION(InvalidOperationException, "failed to find root from inorder vector ...");
        }
        else
        {
            int size_of_left = rank_of_in - in_start;  /*左子树规模*/ 
            int size_of_right = to_process - 1 - size_of_left;  /*右子树规模*/
            /*计算在中序遍历序列中左子树和右子树的起始和结束位置*/
            //cout << "size of left = " << size_of_left << endl;
            //cout << "size of right = " << size_of_right << endl;
            /*构建左子树*/
            if (0 != size_of_left)
            {
                cout << "constructing left tree ..." << endl;
                BinNode<T> *subtree_l = reconstruct(preorder, pre_start + 1, pre_start + size_of_left + 1, inorder, in_start, in_start + size_of_left);

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
                BinNode<T> *subtree_r = reconstruct(preorder, pre_start + size_of_left + 1, pre_end, inorder, in_start + size_of_left + 1, in_end); 

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
void bintree_reconstruction_preorder(Vector<T> &preorder, Vector<T> &inorder)
{
    try
    {
        BinNode<T> *root = reconstruct(preorder, 0, preorder.size(), inorder, 0, inorder.size());

        visit<T> visitor;

        root->traverIn(visitor);
        
        BinTree<T> tree;

        tree.m_root = root;
        tree.m_size = root->size();
    }
    catch (const Exception &obj)
    {
        cout << obj.message() << endl;
        cout << obj.location() << endl;
    }
    catch (...)
    {
        cout << "..." << endl;
    }
}

template < typename T >
BinNode<T> *reconstruct_by_post(Vector<T> &postorder, int post_start, int post_end, Vector<T> &inorder, int in_start, int in_end)
{
    int process_count = post_end - post_start;  /*待处理数组长度*/
    BinNode<T> *ret = NULL;

    ::std::cout << "poststart = " << post_start << ", postend = " << post_end << ", instart = " << in_start << ", inend = " << in_end << ::std::endl;

    if (1 == process_count)
    {
        /*到达递归基,只有一个节点了,这个节点必然是整棵二叉树的叶节点*/
        ret = new BinNode<T> (postorder[post_start]);
    }
    else
    {
        /*否则先从后序遍历数组中定位根节点,即后序遍历数组的最后一个元素*/
        const T &root = postorder[post_end - 1];
        /*在中序遍历数组中定位根节点的位置*/
        int rank_of_root = inorder.find(in_start, in_end, root);
        if (-1 == rank_of_root)
        {
            ::std::cout << "there is something wrong ..." << ::std::endl;
            throw("there is something wrong ...");  /*一定会找到的才对*/
        }

        ret = new BinNode<T> (postorder[post_end - 1]);
        /*分别计算左右子树的规模*/
        int left_size = rank_of_root - in_start;
        int right_size = process_count - left_size - 1;

        ::std::cout << "left size = " << left_size << ::std::endl;
        ::std::cout << "right size = " << right_size << ::std::endl;


        if (left_size > 0)
        {

            ::std::cout << "reconstructing left tree ..." << ::std::endl;
            BinNode<T> *subtree_l = reconstruct_by_post(postorder, post_start, post_start + left_size,
                                            inorder, in_start, in_start + left_size);
            ret->m_leftchild = subtree_l;
            subtree_l->m_parent = ret;
        }
        else
        {
            ::std::cout << "left tree is NULL" << ::std::endl;
            ret->m_leftchild = NULL;
        }

        if (right_size > 0)
        {
            ::std::cout << "reconstructing right tree ..." << ::std::endl;
            BinNode<T> *subtree_r = reconstruct_by_post(postorder, post_start + left_size, post_end - 1,
                                            inorder, in_start + left_size + 1, in_end);
            ret->m_rightchild = subtree_r;
            subtree_r->m_parent = ret;
        }
        else
        {
            ::std::cout << "right tree is NULL" << ::std::endl;
            ret->m_rightchild = NULL;
        }

    }

    return ret;
}

/*通过后序遍历和中序遍历还原二叉树结构*/
template < typename T >
void bintree_reconstruction_postorder(Vector<T> &postorder, Vector<T> &inorder)
{

    try
    {
        BinNode<T> *root = reconstruct_by_post(postorder, 0, postorder.size(), inorder, 0, inorder.size());

        visit<T> visitor;

        root->traverLevel(visitor);
        
        BinTree<T> tree;

        tree.m_root = root;
        tree.m_size = root->size();
    }
    catch (const Exception &obj)
    {
        cout << obj.message() << endl;
        cout << obj.location() << endl;
    }
    catch (...)
    {
        cout << "..." << endl;
    }
}

int main(void)
{

    //MyLib::Vector<int> preorder;
    MyLib::Vector<int> inorder;
    MyLib::Vector<int> postorder;
#if 0
    preorder.insert(1);
    preorder.insert(2);
    preorder.insert(4);
    preorder.insert(7);
    preorder.insert(3);
    preorder.insert(5);
    preorder.insert(6);
    preorder.insert(8);
#endif
    inorder.insert(4);
    inorder.insert(7);
    inorder.insert(2);
    inorder.insert(1);
    inorder.insert(5);

    postorder.insert(7);
    postorder.insert(4);
    postorder.insert(2);
    postorder.insert(5);
    postorder.insert(1);


    //bintree_reconstruction_preorder<int>(preorder, inorder);
    bintree_reconstruction_postorder(postorder, inorder);

    MyLib::TrieTree t;

    t.insert("abc");
    t.insert("b");
    t.insert("aa");
    t.insert("basg");

    ::std::cout << t.find("abc") << ::std::endl;
    ::std::cout << t.find("a") << ::std::endl;
    ::std::cout << t.find("dav") << ::std::endl;

    t.remove("aa");
    ::std::cout << t.find("aa") << ::std::endl;

    return 0;
}

