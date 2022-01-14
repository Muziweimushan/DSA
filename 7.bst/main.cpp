#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <vector>

#include "BST.h"

using namespace MyLib;

struct visit
{
    void operator () (int& v) const
    {
        ::std::cout << v << ::std::endl;
    }
};

static void insert_item(BST<int> &bst)
{
    ::std::vector<int> vec = {2, 4, 5, 8, 10, 11, 13, 15, 16, 17, 19, 22, 25, 27, 28, 33, 37};

    try
    {
        for (auto &iter : vec)
            bst.insert(iter);
    }
    catch (const Exception &err)
    {
        ::std::cout << err.message() << ::std::endl;
    }
    catch (...)
    {
        ::std::cout << "insert failed ..." << ::std::endl;
    }
}


int main(void)
{
    BST<int> bst1;

    insert_item(bst1);

    bst1.travIn(visit());

    ::std::cout << "travLevel : " << ::std::endl;

    bst1.travLevel(visit());

    BinNodePosi(int) del = bst1.remove(2);
    if (nullptr != del)
    {
        ::std::cout << "remove success! ..." << ::std::endl;
        delete del, del = nullptr;
    }
    else
    {
        ::std::cout << "remove failed ..." << ::std::endl;
    }

    return 0;
}
