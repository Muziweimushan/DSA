#pragma once

#ifndef __LIST_H__
#define __LIST_H__

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "ListNode.h"

namespace MyLib
{

    template < typename T >
        class List : public Object
    {
        public:
            /*构造函数*/
            List();
            List(Posi(T) p, int n);	/*调用者必须保证p是合法的,且p的n - 1个后继也是合法的*/
            List(const List<T> &l);	/*拷贝构造函数*/
            List(const List<T> &l, Rank r, int n);	/*拷贝l[r, r + n)中的元素*/

            List(int n, const T &e = 0)
            {
                init();
                if (n > 0)
                    for (int i = 0; i < n; i++)
                        insertAsLast(e);
            }

            List &operator = (const List<T> &obj)
            {
                //::std::cout << "List<T>::operator = " << ::std::endl;
                if (this != &obj)
                {
                    /*先释放原有的所有节点*/
                    while (m_size > 0)
                        remove(0);

                    for (int i = 0; i < obj.m_size; i++)
                        insertAsLast(obj[i]);
                }

                return *this;
            }

            /*只读访问接口*/
            Rank size() const;    /*获取列表规模*/
            bool empty() const; /*判空*/
            T &operator [] (Rank r) const;  /*支持循秩访问,效率低,时间复杂度为O(r), 平均效率为O(n)*/
            Posi(T) first() const;  /*返回首节点位置,当size为0时为空*/
            Posi(T) last() const;   /*返回末节点位置*/
            bool valid(Posi(T) p) const;    /*判断位置p是否在当前列表中是否是合法位置*/

            /*无序列表的查找,返回NULL表示没找到*/
            /*无序列表的查找操作find,在最好的情况下复杂度为O(1),最坏的情况下为O(n)*/
            Posi(T) find(const T &e) const; /*无序列表的查找*/
            Posi(T) find(const T &e, int n, Posi(T) p) const; /*在无序列表中元素p的前n个前驱(不包括p本身)中查找元素e*/
            Posi(T) find(const T &e, Posi(T) p, int n) const; /*在无序列表中元素p的后n个后继(不包括p本身)中查找元素e*/

            /*有序列表的查找,由于列表本身遍历只能沿一个方向一个一个节点遍历,所以不管列表是否有序查找的复杂度都无法降低,这是与向量不一样的*/
            /*整个列表的查找,返回最后一个不大于e的元素的位置,调用者需要通过valid接口判断返回值是否合法*/
            Posi(T) search(const T &e) const;

            /*在有序列表中元素p的前n个前驱(不包括p本身)中查找元素e, 返回最后一个不大于e的元素的节点位置*/
            Posi(T) search(const T &e, int n, Posi(T) p) const;

            /*在有序列表中元素p的后n个后继(不包括p本身)中查找元素e, 返回第一个不小于e的元素的节点位置*/
            Posi(T) search(const T &e, Posi(T) p, int n) const; 

            /*在无序列表中,在位置p以及其n - 1个后继中选出最大者, 如果有多个,则返回秩最大的节点所在的位置*/
            Posi(T) selectMax(Posi(T) p, int n) const;
            Posi(T) selectMax() const;    /*在无序列表中找出整体最大,如有歧义,则后者优先*/

            int disordered() const;	/*判断列表是否有序,返回0表示不存在逆序的一对节点,也就是有序*/

            /*可写访问接口*/
            Posi(T) insertAsFirst(const T &e);  /*作为首节点插入*/
            Posi(T) insertAsLast(const T &e);   /*作为末节点插入*/
            Posi(T) insertBefore(Posi(T) p, const T &e);    /*插入到p的前驱位置*/
            Posi(T) insertAfter(Posi(T) p, const T &e); /*插入到p的后继位置*/
            T remove(Posi(T) p);    /*删除合法位置p的节点,并返回被删除的元素*/
            T remove(Rank r);
            void merge(List<T> &list); /*全列表做归并*/
            void sort(Posi(T) p, int n);    /*列表区间排序*/
            void sort();    /*列表整体排序*/

            int deduplicate();  /*无序列表的去重操作*/
            int uniquify(); /*有序列表的去重操作*/
            void reverse(); /*列表的前后倒序*/
            void reverse_V3();

            void traverse(void (*visit)(T &e));  /*遍历操作*/
            template < typename VST >
                void traverse(const VST &visit);

            ~List();
        protected:
            void init(void);
            int clear(); /*清除所有节点,返回原列表的规模*/
            void copyNodes(Posi(T) p, int n);   /*复制列表中自p开始的n项*/
            void merge(Posi(T) &p1, int n1, List<T> &l, Posi(T) p2, int n2);   /*二路归并*/
            void mergeSort(Posi(T) &p, int n);   /*归并排序,对从p开始连续n个节点进行归并排序*/
            void selectionSort(Posi(T) p, int n);  /*选择排序,对从p开始连续n个节点进行选择排序*/
            void insertionSort(Posi(T) p, int n);  /*插入排序,对从p开始连续的n个节点进行插入排序*/
            //void mergeSortInner(Posi(T) &p, int n);
            //private:
        public:
            Rank m_size;/*列表的规模*/
            /*
             * 以下的数据类型与ListNode列表节点完全一致,
             * 用这个数据类型定义头节点和末节点,避免泛指类型T在构造时可能出错的问题,
             * 关键就是记得需要继承自Object顶层父类,
             * 否则就会与ListNode类对象的内存排布不一样了
             */
            struct InnerNodeType : public Object
        {
            /*vtable...*/
            Posi(T) pred;
            Posi(T) succ;
            char reserved[sizeof(T)];
        };
            Posi(T) m_header;	/*头节点*/
            Posi(T) m_trailer;	/*末节点*/
            /*与此对应的第一个有效的节点称为首节点,最后一个有效的节点称为尾节点, 头、首、尾、末节点的秩分别为-1、0、(size - 1) 和 size*/

            InnerNodeType m_node_header;
            InnerNodeType m_node_trailer;
            /*
             * 获取节点位置p的节点在当前列表中的秩
             * 对于有效节点(visible),它们的rank返回值为[0, size() - 1]
             * 对于列表中所有节点,他们的rank接口返回值合法范围是[-1, size()],-1表明当前节点是头节点,size()表明当前节点是末节点
             */
            Rank rank(Posi(T) p) const;

            /*
             * added by liweibin:
             * 下面remove和insert接口不在书中,主要用于排序时,当泛指类型T的拷贝操作很耗时时,排序时经常remove和insert就很耗时了,而且伴随着节点的new和delete,这里增加remove接口,使得删除时将节点从列表中删除,但是节点不释放,而是作为返回值返回,插入就类比了,可以不需要new操作,仅用于内部用
             */
            Posi(T) removeFromList(Posi(T) p);
            /*将节点n作为p的前驱插入到当前列表中*/
            Posi(T) insertBefore(Posi(T) pos, Posi(T) n);
            Posi(T) insertAfter(Posi(T) p, Posi(T) n);
    };

#include "List_implementation.h"


}

#endif
