#include <iostream>
#include <cstdlib>
#include <math.h>
#include <cstring>
#include <cstdio>

#include "BinNode.h"
#include "BinTree.h"
#include "Exception.h"

using namespace std;

void test(MyLib::BinTree<int> &tree, MyLib::BinTree<int> *tree2)
{
    tree.attachAsLeftChild(tree.root(), tree2);
}

template < typename T >
void bintree_reconstruction_preorder(MyLib::Vector<T> &preorder, MyLib::Vector<T> &inorder);

int main(void)
{
    MyLib::Vector<int> preorder;
    MyLib::Vector<int> inorder;

    preorder.insert(1);
    preorder.insert(2);
    preorder.insert(4);
    preorder.insert(7);
    preorder.insert(3);
    preorder.insert(5);
    preorder.insert(6);
    preorder.insert(8);

    inorder.insert(4);
    inorder.insert(7);
    inorder.insert(2);
    inorder.insert(1);
    inorder.insert(5);
    inorder.insert(3);
    inorder.insert(8);
    inorder.insert(6);
    bintree_reconstruction_preorder(preorder, inorder);

#if 0
    MyLib::BinTree<int> tree;

    tree.insertAsRoot(0);
    tree.insertAsLeftChild(tree.root(), 1);
    tree.insertAsRightChild(tree.root(), 2);

    MyLib::BinNodePosi(int) r1 = tree.root()->m_leftchild;
    r1->insertAsLeftChild(3);

    MyLib::BinNodePosi(int) r2 = tree.root()->m_rightchild;
    r2->insertAsRightChild(5);

    visit visitor;

    //tree.travIn(visitor);
    tree.travPost(visitor);
#endif
#if 0
    MyLib::BinTree<int> *new_tree = tree.secede(r1);

    delete new_tree, new_tree = NULL;

    tree.travPre(visitor);
#endif
    return 0;
}
