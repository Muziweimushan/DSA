#pragma once

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "Object.h"

namespace MyLib
{

/*定义遍历过程中顶点状态*/
typedef enum
{
    VERTEX_STAT_UNDISCOVERED = 0x0,
    VERTEX_STAT_DISCOVERED = 0x1,
    VERTEX_STAT_VISITED = 0x2,
    VERTEX_STATUS_CNT
}VERTEX_STATUS_E;

/*定义边在遍历树中所属的类型*/
typedef enum
{
    EDGE_TYPE_UNDETERMINED = 0x0,
    EDGE_TYPE_TREE = 0x1,
    EDGE_TYPE_CROSS = 0x2,
    EDGE_TYPE_FORWARD = 0x3,
    EDGE_TYPE_BACKARD = 0x4,
    EDGE_TYPE_CNT
}EDGE_TYPE_E;

/*定义抽象模板类Graph,给出相关接口ADT*/
template < typename Tv, typename Te >
class Graph : public Object
{
public:
    /*****************顶点操作ADT****************************/

    /*获取图中顶点数*/
    int getVertexCnt(void) const;

    /*获取顶点数据域*/
    virtual Tv &vertex(int) = 0;

    /*插入顶点,返回编号*/
    virtual int insert(const Tv &) = 0;

    /*删除顶点及其关联边,返回顶点的数据*/
    virtual Tv remove(int) = 0;

    /*获取顶点v的入度*/
    virtual int inDegree(int) = 0;

    /*获取顶点v的出度*/
    virtual int outDegree(int) = 0;

    /*获取顶点v的状态*/
    virtual VERTEX_STATUS_E &status(int) = 0;

    /*顶点v的时间标签dTime*/
    virtual int &dTime(int) = 0;

    /*顶点v的时间标签fTime*/
    virtual int &fTime(int) = 0;

    /*获取顶点v在遍历树中的父亲*/
    virtual int &parent(int) = 0;

    /*顶点v在遍历树中的优先级数*/
    virtual int &priority(int) = 0;

    /*顶点v的首个邻接顶点*/
    virtual int firstNbr(int) = 0;

    /*顶点v的(相对于顶点j的)下一个邻接顶点,如果没有,则返回-1*/
    virtual int nextNbr(int, int) = 0;

    /*******************边操作ADT*****************************/

    /*获取图中边数*/
    int getEdgeCnt(void) const;

    /*在顶点v和u之间插入一条权重为w的边e*/
    virtual void insert(const Te &, int, int, int) = 0;

    /*删除顶点v和u之间的边e,并返回e的数据域*/
    virtual Te remove(int, int) = 0;

    /*判断边(v, u)是否存在*/
    virtual bool exist(int, int) const = 0;

    /*边(v, u)的类型*/
    virtual EDGE_TYPE_E &type(int, int) = 0;

    /*获取边(v, u)的权重*/
    virtual int &weight(int, int) = 0;

    /*获取某一条边(u, v)的数据域*/
    virtual Te &edge(int, int) = 0;

    /********************图算法统一ADT***********************/

private:
    int m_edge_cnt;     /*边总数*/
    int m_vertex_cnt;   /*顶点总数*/

    /*复位所有边和顶点的辅助信息*/
    void reset(void);
};

template < typename Tv, typename Te >
void Graph<Tv, Te>::reset(void)
{
    /*现在还没有什么辅助信息,暂时啥也不干*/
}

template < typename Tv, typename Te >
int Graph<Tv, Te>::getEdgeCnt(void) const
{
    return m_edge_cnt;
}

template < typename Tv, typename Te >
int Graph<Tv, Te>::getVertexCnt(void) const
{
    return m_vertex_cnt;
}

}


#endif
