#include "Fib.h"

using namespace MyLib;


Fib::Fib(int n)
{
    m_a = 0;
    m_b = 1;

    while (n > m_b)
    {
        m_b = m_a + m_b; 
        m_a = m_b - m_a;
    }
}

int Fib::get()
{
    return m_b;
}

int Fib::next()
{
    m_b = m_a + m_b;
    m_a = m_b - m_a;

    return m_b;
}

int Fib::prev()
{
    m_a = m_b - m_a;
    m_b = m_b - m_a;

    return m_b;
}
