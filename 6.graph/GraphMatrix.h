#pragma once

#ifndef __GRAPHMATRIX_H__
#define __GRAPHMATRIX_H__

#include <climits>
#include "Graph.h"
#include "Vector.h"

namespace MyLib

{

template < typename Tv >
class Vertex : public Object
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

    Vertex(const Tv &data)
        : m_data(data), m_inDegree(0), m_outDegree(0), m_status(VERTEX_STAT_UNDISCOVERED),
        m_dTime(-1), m_fTime(-1), m_parent(-1), m_priority(INT_MAX)
    {
    }
};

/*边数据类型ADT*/
template < typename Te >
class Edge : public Object
{
public:
    Te m_data;  /*边数据域*/
    int m_weight;   /*权重*/
    EDGE_TYPE_E m_type; /*在遍历树中所属的类型*/

    /*构造函数,构造一条新的边*/
    Edge(const Te &data, int weight)
        : m_data(data), m_weight(weight), m_type(EDGE_TYPE_UNDETERMINED)
    {
    }
};

/*使用邻接矩阵表示的图的ADT实现*/
template < typename Tv, typename Te >
class GraphMatrix : public Graph<Tv, Te>
{
public:
    /*构造函数*/
    GraphMatrix()
        : Graph<Tv, Te>::m_edge_cnt(0), Graph<Tv, Te>::m_vertex_cnt(0)
    {
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






    ~GraphMatrix()
    {
        for (int i = 0; i < this->m_vertex_cnt; i++)
            for (int j = 0; j < this->m_vertex_cnt; j++)
                delete m_edge_set[i][j];
    }

private:
    /*顶点集,使用向量进行存储,即 a vector of vertices*/
    Vector< Vertex<Tv> > m_vertex_vec;
    /*边集,同时也是邻接矩阵,是一个二维向量,即 A vertor of vectors of edges*/
    Vector< Vector< Edge<Te> * > > m_edge_set;  /*边的集合*/
};

}

#endif
