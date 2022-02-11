#pragma once

#ifndef __RANGEARRAY_H__
#define __RANGEARRAY_H__

#include "Array.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class RangeArray : public Array<T>
{
public:
    RangeArray(int lower, int upper)
    {
        if (upper < lower)
          THROW_EXCEPTION(InvalidParameterException, "Initializing RangeArray with invalid range ...");

        this->m_array = new T[upper - lower + 1];
        if (nullptr != this->m_array)
        {
            m_upper = upper;
            m_lower = lower;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create RangeArray object ...");
        }
    }

    RangeArray(const RangeArray<T> &obj)
    {
        int len = obj.m_upper - obj.m_lower + 1;

        T *arr = new T[len];
        if (nullptr != arr)
        {
            this->m_array = arr;
            for (int i = 0; i < len; i++)
                this->m_array[i] = obj.m_array[i];

            m_upper = obj.m_upper;
            m_lower = obj.m_lower;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create RangeArray object ...");
        } 
    }

    RangeArray<T> &operator = (const RangeArray<T> &obj)
    {
        if (this != &obj)
        {
            int len = obj.length();
            T *arr = new T[len];
            if (nullptr != arr)
            {
                for (int i = 0; i < len; i++)
                    arr[i] = obj.m_array[i];

                m_upper = obj.m_upper;
                m_lower = obj.m_lower;

                T *toDel = this->m_array;
                this->m_array = arr;

                delete toDel;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create RangeArray object ...");
            } 
        }
    }

    bool set(int i, const T &e)
    {
        return Array<T>::set(i - m_lower, e);
    }

    bool get(int i, T &e)
    {
        return Array<T>::get(i - m_lower, e); 
    }

    T &operator [] (int i)
    {
        return (static_cast<Array<T> &>(*this))[i - m_lower];
    }

    T operator [] (int i) const
    {
        return const_cast<RangeArray<T> &>(*this)[i];
    }

    int upper(void) const { return m_upper; }
    int lower(void) const { return m_lower; }

    int length() const { return m_upper - m_lower + 1; }

    ~RangeArray()
    {
    }

private:
    int m_upper;    /*上界*/
    int m_lower;    /*下界*/

};

}


#endif
