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

class visit
{
public:
    void operator ()(int &data) const
    {
        cout << data << endl;
    }
};


int main(void)
{
    MyLib::BinTree<int> tree;

    tree.insertAsRoot(0);
    tree.insertAsLeftChild(tree.root(), 1);
    tree.insertAsRightChild(tree.root(), 2);

    MyLib::BinNodePosi(int) r1 = tree.root()->m_leftchild;
    r1->insertAsLeftChild(3);

    MyLib::BinNodePosi(int) r2 = tree.root()->m_rightchild;
    r2->insertAsRightChild(5);

    visit visitor;

    tree.travIn(visitor);
#if 0
    MyLib::BinTree<int> *new_tree = tree.secede(r1);

    delete new_tree, new_tree = NULL;

    tree.travPre(visitor);
#endif
    return 0;
}
