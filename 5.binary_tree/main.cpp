#include <iostream>
#include <cstdlib>
#include <math.h>
#include <cstring>
#include <cstdio>

#include "BinNode.h"
#include "Exception.h"

using namespace std;


int main(void)
{
    MyLib::BinNode<int> b;

    cout << b.m_data << endl;
    return 0;
}
