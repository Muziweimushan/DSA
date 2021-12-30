#pragma once

#ifndef __LISTGRAPH_H__
#define __LISTGRAPH_H__

#include <climits>

#include "Graph.h"
#include "Vector.h"
#include "List.h"

namespace MyLib

{

template < typename Tv >
class ListVertex : public Object
{
public:
    Tv m_data;  /*顶点数据域*/
    int m_inDegree; /*入度*/
    int m_outDegree;    /*出度*/
    VERTEX_STATUS_E m_status;   /*状态*/
    /*时间标签*/
    int m_dTime;
    int m_fTime;
    int m_parent; /*在遍历树中的父节点*/
    int m_priority; /*在遍历树中的优先级(最短通路、极短跨边等)*/

    ListVertex(const Tv &data = (Tv)0)
        : m_data(data), m_inDegree(0), m_outDegree(0), m_status(VERTEX_STAT_UNDISCOVERED),
        m_dTime(-1), m_fTime(-1), m_parent(-1), m_priority(INT_MAX)
    {
    }
};

/*边数据类型ADT*/
template < typename Te >
class ListEdge : public Object
{
public:
    Te m_data;  /*边数据域*/
    int m_weight;   /*权重*/
    EDGE_TYPE_E m_type; /*在遍历树中所属的类型*/
    int m_vertex_num;  /*关联顶点*/

    /*构造函数,构造一条新的边*/
    ListEdge(const Te &data, int weight)
        : m_data(data), m_weight(weight), m_type(EDGE_TYPE_UNDETERMINED)
    {
    }

    bool operator == (ListEdge<Te> *&obj) const
    {   
        ::std::cout << "operator == of ListEdge ..." << ::endl;
        return this->m_vertex_num == obj->m_vertex_num;
    }
};


/*使用邻接表表示的图的ADT实现*/
template < typename Tv, typename Te >
class ListGraph : public Graph<Tv, Te>
{
public:
    /*构造函数*/
    ListGraph()
    {
        this->m_edge_cnt = 0;
        this->m_vertex_cnt = 0;
    }

    /*顶点相关接口实现*/
    /*获取编号为i的顶点的数据域*/
    Tv &vertex(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_data;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*入度*/
    int inDegree(int i)
    {
    
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_inDegree;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*出度*/
    int outDegree(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_outDegree;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*获取顶点状态*/
    VERTEX_STATUS_E &status(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_status;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*获取顶点时间标签dTime*/
    int &dTime(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_dTime;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*获取顶点时间标签fTime*/
    int &fTime(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_fTime;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*获取顶点在遍历树中的父亲*/
    int &parent(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_parent;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*获取顶点v在遍历树中的优先级*/
    int &priority(int i)
    {
        if (0 <= i && i < this->m_vertex_cnt)
            return m_vertex_vec[i].m_priority;
        else
            THROW_EXCEPTION(IndexOutOfBoundException, "Rank of vertex is out-of-range ...");
    }

    /*若已枚举至邻居j,则转向下一邻居*/
    int nextNbr(int i, int j)
    {
        Posi(ListEdge<Te> *) edge = position(i, j);
        /*通过list节点的succ指针访问下一个节点,之后通过List的valid接口判断下一个节点是否合法*/
        Posi(ListEdge<Te> *) next = edge->succ();
        if (m_edge_set[i].valid(next))
        {
            return next->data()->m_vertex_num;
        }
        else
            return -1;  /*说明当前顶点j已经是邻接表中第i行的最后一个元素了*/
    }

    /*查找顶点i的第一个邻居*/
    int firstNbr(int i)
    {
        int ret = -1;   /*这里是访问顶点的编号*/
        /*改用邻接表以后直接访问list的首元素即可*/
        if (m_edge_set[i].size() > 0)
            ret = m_edge_set[i][0]->m_vertex_num; 
        else
            ret = -1;

        //::std::cout << "firNbr = " << ret << ::std::endl;

        return ret;
    }

    /*插入一个顶点,返回顶点的编号*/
    int insert(const Tv &data)
    {
        /*首先邻接表需要新增一行,元素类型为list of edge*/
        List< ListEdge<Te> * > tmp;

        //::std::cout << "header of tmp = " <<  tmp.m_header << ::std::endl;
        // ::printf("tmp : succ of header = %p\n", tmp.m_header->m_succ);
        // ::printf("tmp : pred of header = %p\n", tmp.m_header->m_pred);
        // ::printf("tmp : succ of trailer = %p\n", tmp.m_trailer->m_succ);
        // ::printf("tmp : pred of trailer = %p\n", tmp.m_trailer->m_pred);


        //::printf("\n\n\ncut here ...\n\n\n");

        m_edge_set.insert(tmp);

        //::std::cout << "edge set insert ..." << ::std::endl;
        //::std::cout << "set size = " << m_edge_set.size() << ::std::endl;

        List< ListEdge<Te> * > &list = m_edge_set[0];
#if 0
        ::std::cout << "addr of inner list = " << &list << ::std::endl;

        ::printf("inner list : succ of header = %p\n", list.m_header->m_succ);
        ::printf("inner list : pred of header = %p\n", list.m_header->m_pred);
        ::printf("inner list : succ of trailer = %p\n", list.m_trailer->m_succ);
        ::printf("inner list : pred of trailer = %p\n", list.m_trailer->m_pred);


        ::printf("\n\n\ncut here ...\n\n\n");
#endif
        /*顶点个数增加1*/
        this->m_vertex_cnt++;

        /*最后再顶点集合中新增一个元素*/
        return m_vertex_vec.insert(ListVertex<Tv>(data));
    }

    /*删除顶点i以及它的所有关联边,返回顶点i的数据域*/
    Tv remove(int i)
    {
        /*与顶点插入对应*/
        /*删除邻接矩阵的第i行*/
        while (m_edge_set[i].size() > 0)
        {
            /*删除顶点i的所有出边*/
            Posi(ListEdge<Te> *) pos = m_edge_set[i].first();

            ListEdge<Te> *toDel = m_edge_set[i].remove(pos);
            int vertex_num = toDel->m_vertex_num;
            m_vertex_vec[vertex_num].m_inDegree--;
            this->m_edge_cnt--;
        
            delete toDel;
        }

        /*可以将这一行删掉了*/
        m_edge_set.remove(i);
        /*顶点数减一*/
        this->m_vertex_cnt--;

        /*需要在顶点集中先将第i个元素删掉,因为后面访问顶点集时index以删除后的index为准的*/
        Tv ret = vertex(i);
        m_vertex_vec.remove(i);

        /*检查顶点i的入边*/
        for (int j = 0; j < this->m_vertex_cnt; j++)
        {
            Posi(ListEdge<Te> *) edge = position(j, i);
            if (NULL != edge)
            {
                /*存在一条从j指向i的边,删掉它*/
                ListEdge<Te> *toDel = m_edge_set[j].remove(edge);
                m_vertex_vec[j].m_outDegree--;  /*顶点j的出度减一*/
                this->m_edge_cnt--; /*图的边数减一*/

                delete toDel;
            }
        }

        return ret;
    }



    /************************************边相关操作定义****************************************/

    /*判断边(i, j)是否存在*/
    bool exist(int i, int j) const
    {
        /*采用邻接表也就是list之后,不能像使用vector直接按秩访问,必须遍历*/
        Posi(ListEdge<Te> *) result = position(i, j);
        
        return (NULL != result);
    }

    /*边(i, j)的数据域*/
    Te &edge(int i, int j)
    {
        ::std::cout << "ListGraph::edge ..." << ::std::endl;
        Posi(ListEdge<Te> *) edge = position(i, j);
        if (NULL != edge)
            return edge->m_data->m_data;
        else
            THROW_EXCEPTION(InvalidOperationException, "Edge is not exist ...");
    }
    
    /*边(i, j)的类型*/
    EDGE_TYPE_E &type(int i, int j)
    {
        Posi(ListEdge<Te> *) edge = position(i, j);
        if (NULL != edge)
            return edge->m_data->m_type;
        else
        {
            ::printf("find edge from %d to %d failed ...\n", i, j);
            THROW_EXCEPTION(InvalidOperationException, "Edge is not exist ...");
        }
    }

    /*边(i, j)的权重*/
    int &weight(int i, int j)
    {
        Posi(ListEdge<Te> *) edge = position(i, j);
        if (NULL != edge)
            return edge->m_data->m_weight;
        else
            THROW_EXCEPTION(InvalidOperationException, "Edge is not exist ...");
    }

    /*在顶点i和顶点j之间联接一条权重为w的边e*/
    void insert(const Te &data, int i, int j, int w)
    {
        if (exist(i, j))
            THROW_EXCEPTION(InvalidOperationException, "edge already exist ...");

        ::printf("inserting edge from %d to %d ...\n", i, j);

        ListEdge<Te> *e = new ListEdge<Te>(data, w);
        if (NULL != e)
        {
            e->m_vertex_num = j;    /*关联顶点编号*/
            //::printf("lalala\n");
            m_edge_set[i].insertAsLast(e);
            //::printf("endof lalala ...\n");
            this->m_edge_cnt++; /*图的边数量加一*/
            m_vertex_vec[i].m_outDegree++;  /*顶点i的出度加一*/
            m_vertex_vec[j].m_inDegree++;   /*顶点j的入度加一*/
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new edge object ...");
        }
    }
    
    /*删除顶点i与顶点j之间的联边*/
    Te remove(int i, int j)
    {
        //if (!exist(i, j))
            //THROW_EXCEPTION(InvalidOperationException, "No edge exist ...");

        Posi(ListEdge<Te> *) pos = position(i, j);
        if (NULL == pos)
            THROW_EXCEPTION(InvalidOperationException, "No edge exist ...");

        ListEdge<Te> *toDel = m_edge_set[i].remove(pos);
        pos = NULL;

        /*更新图内部信息*/
        this->m_edge_cnt--;
        m_vertex_vec[i].m_outDegree--;
        m_vertex_vec[j].m_inDegree--;

        Te ret = toDel->m_data;

        /*异常安全*/
        delete toDel;

        return ret;
    }

    ~ListGraph()
    {
        for (int i = 0; i < this->m_vertex_cnt; i++)
        {
            while (m_edge_set[i].size() > 0)
            {
                ListEdge<Te> *toDel = m_edge_set[i].remove(0);
                delete toDel;
            }
        }
    }

public:
    /*顶点集,使用向量进行存储,即 a vector of vertices*/
    Vector< ListVertex<Tv> > m_vertex_vec;
    /*边集,同时也是邻接矩阵,采用邻接表的方式实现,即 A vertor of lists of edges*/
    Vector< List< ListEdge<Te> * > > m_edge_set;  /*边的集合*/

    /*功能函数:返回边(i, j)的ListEdge指针对象*/
    Posi(ListEdge<Te> *) position(int i, int j) const
    {
        /*首先检查i和j的合法性*/
        if (i < 0 || i >= this->m_vertex_cnt || j < 0 || j >= this->m_vertex_cnt)
        {
            //::printf("position invalid ...\n");
            THROW_EXCEPTION(IndexOutOfBoundException, "Index of vertex is invalid ...");
        }

        //::printf("calling position, i = %d, j = %d\n", i, j);
        //const List< ListEdge<Te> *> &list = m_edge_set[0];
        //::std::cout << "set[0].succ = "<< list.m_trailer->m_succ << ::std::endl;

        Posi(ListEdge<Te> *) ret = m_edge_set[i].first();
        int n = 0;

        for (n = 0; n < m_edge_set[i].size(); n++)
        {
             if (j == ret->m_data->m_vertex_num)
             {
                //::printf("find one edge ...\n");
                break;
             }
             ret = ret->m_succ;
        }
        if (m_edge_set[i].size() == n)
            ret = NULL;

        //::printf("n = %d\n", n);

        return ret;
    }
};

}

#endif
