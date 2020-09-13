#ifndef __FAB_H__
#define __FAB_H__

#include "Object.h"

namespace MyLib

{

class Fib : public Object
{
public :
    Fib(int n);
    int get();
    int next();
    int prev();

private:
    int m_a;
    int m_b;
};

}

#endif
