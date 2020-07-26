#pragma once

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "List.h"

namespace MyLib
{

template < typename T >
class Queue : public List<T>
{
public:

    /*入队列操作*/
    void enqueue(const T &e)
    {
        this->insertAsLast(e);    /*将元素e作为列表尾节点插入即可*/
    }

    T dequeue()
    {
        return this->remove(0);   /*将列表首节点移除*/
    }

    T &front()
    {
        return this->first()->m_data;
    }
};


}



#endif