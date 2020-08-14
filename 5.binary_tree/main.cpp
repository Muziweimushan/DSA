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


int main(void)
{
    MyLib::BinNode<int> b;

    b.insertAsLeftChild(1);
    b.insertAsRightChild(2);
#if 0
    cout << b.m_data << endl;

    cout << b.m_leftchild->m_data << endl;
    cout << b.m_rightchild->m_data << endl;

    cout << b.size() << endl;

    const MyLib::BinNode<int> &rb = *(b.m_leftchild);
    cout << IsRChild(rb) << endl;
    cout << IsRoot(b) << endl;
    cout << IsRoot(rb) << endl;
    cout << HasParent(b) << endl;
    cout << HasParent(rb) << endl;
#endif

    MyLib::BinTree<int> tree;
    tree.insertAsRoot(11);
    cout << tree.root()->m_height << endl;   
    //tree.insertAsLeftChild(tree.root(), 12);
    tree.insertAsRightChild(tree.root(), 13);

    cout << tree.root()->m_height << endl;   
    
    MyLib::BinTree<int> tree2;
    tree2.insertAsRoot(1);
    tree2.insertAsLeftChild(tree2.root(), 12);
    tree2.insertAsRightChild(tree2.root(), 13);

    //test(tree, &tree2);
    return 0;
}
