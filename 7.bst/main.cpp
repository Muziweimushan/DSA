#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <vector>

#include "BST.h"

using namespace MyLib;

class Test
{
public:
    Test(int first, int second = 0) : m_first(first), m_second(second) {}

    void print(void) { ::std::cout << m_first << ", " << m_second << ::std::endl; }

    bool operator < (const Test &rhs) const
    {
        return (this->m_first < rhs.m_first);
    }

    bool operator == (const Test &rhs) const
    {
        return (this->m_first == rhs.m_first);
    }
private:
    int m_first;
    int m_second;
};

struct myVisit
{
    void operator () (Test &t) const { t.print(); }
};

struct visit
{
    void operator () (int& v) const
    {
        ::std::cout << v << ::std::endl;
    }
};

static void insert_item(BST<int> &bst)
{
    ::std::vector<int> vec = {16, 10, 25, 5, 11, 19, 28, 2, 8, 15, 17, 22, 27, 37, 4, 13, 33};
    //::std::vector<int> vec = {16, 10, 25, 5, 11};
    try
    {
        for (auto &iter : vec)
        {
            ::std::cout << "inserting value : " << iter << ::std::endl; 
            bst.insert_dup(iter);
        }
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

static void insert_class_item(BST<Test> &bst)
{
    ::std::vector<int> vec = {16, 10, 25, 5, 11, 19, 28, 2, 5, 8, 15, 17, 22, 27, 5, 37, 4, 13, 33};
    //::std::vector<int> vec = {16, 10, 25, 5, 11};
    //
    int count = 0;
    try
    {
        for (auto &iter : vec)
        {
            ::std::cout << "inserting value : " << iter << ::std::endl; 
            if (5 == iter)
                bst.insert_dup({iter, count++});
            else
                bst.insert_dup({iter, 0});

        }
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
    BST<Test> bst2;

    //insert_item(bst1);
    insert_class_item(bst2);

    bst2.travIn(myVisit());

    ::std::cout << "travLevel : " << ::std::endl;

    bst2.travLevel(myVisit());

    BinNodePosi(Test) del = bst2.remove(Test{2, 0});
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
