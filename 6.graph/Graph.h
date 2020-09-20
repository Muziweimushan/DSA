#pragma once

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "Object.h"
#include "Queue.h"

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
    /*全图的bfs算法,以编号为s的顶点开始*/
    void bfs(int s);

protected:
    int m_edge_cnt;     /*边总数*/
    int m_vertex_cnt;   /*顶点总数*/

private:
    /*复位所有边和顶点的辅助信息*/
    void reset(void);

    /*内部算法接口*/
    /*以v作为起点的bfs-tree遍历算法*/
    /*实际上这里将得到一个覆盖起点s的连通域(连通分量或可达分量)*/
    /*由于整个图算法中可能有多个连通域,且这些连通域之间不相连*/
    /*因此这个接口函数作为整个图算法的bfs算法主体实现*/
    void BFS(int v, int &clock);
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

template < typename Tv, typename Te >
void Graph<Tv, Te>::BFS(int v, int &clock)
{
    /*实现以v作为起点的BFS算法*/
    
    /*BFS类似于二叉树的层次遍历,因此此处也需要使用辅助队列*/
    Queue<int> queue;

    /*初始化工作*/
    status(v) = VERTEX_STAT_DISCOVERED;
    queue.enqueue(v);   /*顶点v入队,将它作为起点*/

    /*这里的流程与二叉树的层次遍历几乎一致*/
    while (!queue.empty())
    {
        int v = queue.dequeue();
        dTime(v) = ++clock;

        /*访问当前顶点v*/
        /*
        *   BFS算法仅仅是一个骨架,提供了对图的一种遍历方法,
        *   在此处实际上需要按实际情况添加对顶点v的数据域进行处理的代码
        */


        /*紧接着*/
        /*依次访问v的所有未被访问的邻接顶点*/
        /*遍历邻接矩阵中的第v行, 遍历顶点v的所有邻接顶点*/
        /*借助firstNbr和nextNbr接口完成,nextNbr将一直返回下一个邻接顶点的编号,如果没有则返回-1*/
        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))    /*访问v的每一个邻居*/
        {
            /*看看当前顶点是否是未访问的*/
            if (VERTEX_STAT_UNDISCOVERED == status(u))
            {
                /*当前顶点u确实是未被访问的,令它入队*/
                /*当然一些辅助信息还是需要填充的*/ 
                parent(u) = v;  /*在BFS Tree中顶点v就是u的父亲了*/
                /*将它入队,状态应改为已发现的,等到它作为队头元素被取出并处理时再将它状态改为已访问*/
                status(u) = VERTEX_STAT_DISCOVERED; 
                /*将从v到u之间的边设置为TREE,即BFS示例中的联边加粗*/
                type(v, u) = EDGE_TYPE_TREE;
                queue.enqueue(u);
            }
            else
            {
                /*否则实际上我们在这一轮迭代中不对顶点u进行处理*/
                /*在这里将顶点v到顶点u之间的联边类型设置为跨边,即示例图中颜色变成浅色*/
                type(v, u) = EDGE_TYPE_CROSS;
            }
        }

        /*最后将顶点v标记为已经访问的*/
        status(v) = VERTEX_STAT_VISITED;
    }
}

template < typename Tv, typename Te >
void Graph<Tv, Te>::bfs(int s)
{
    /*检查s的合法性*/
    if (0 < s || s >= m_vertex_cnt)
        THROW_EXCEPTION(IndexOutOfBoundException, "trying to start bfs from invalid index of vertex ...");

    int clock = 0;  /*最开始时间设置为0*/
    int v = s;

    reset();
   
    /*
    *   基于BFS接口实现能遍历以顶点v作为根节点的bfs-tree中的所有顶点,
    *   但是这颗bfs-tree不一定包含图中所有顶点
    *   因此整个图的BFS算法应当检查所有的顶点,
    *   如果发现一个顶点处于undiscovered状态,则随即以这个顶点为起点调用一次BFS接口
    *   直到所有顶点的状态都不为discovered
    *   此时认为图中所有顶点按照BFS算法顺序进行了一次且仅一次的访问
    */
    do
    {
        if (VERTEX_STAT_UNDISCOVERED == status(v))
            BFS(v, clock);
    } while (s != (v = (++v) % m_vertex_cnt));  /*环形转一圈*/
}

}


#endif
