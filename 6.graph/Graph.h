#pragma once

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "Object.h"
#include "Queue.h"
#include "Stack.h"

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

    /*全图的dfs算法,以编号为s的顶点开始*/
    void dfs(int s);

    /*基于DFS的拓扑排序,将拓扑排序后的各顶点的顺序记录在入参stack中*/
    void tSort(int s, Stack<int> &stack);

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

    void DFS(int v, int &clock);
    void DFS_V2(int v, int &clock);

    void TSort(int v, Stack<int> &stack);
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

        ::std::cout << "process vertex " << vertex(v) << ::std::endl;
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

template < typename Tv, typename Te >
void Graph<Tv, Te>::DFS(int v, int &clock)
{
    /*将v标记为discovered*/
    status(v) = VERTEX_STAT_DISCOVERED;
    dTime(v) = ++clock; /*记录顶点v的发现时间*/

    ::std::cout << "processing vertex " << v << " ..." << ::std::endl;

    /*遍历顶点v的所有邻居*/
    for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
    {
        /*不同状态的邻居进入不同分支分别处理*/
        if (VERTEX_STAT_UNDISCOVERED == status(u))
        {
            /*如果顶点u是未发现的,则应该引入树边*/
            type(v, u) = EDGE_TYPE_TREE;
            parent(u) = v;  /*自然的,在遍历树中v就是u的父节点了*/
            DFS(u, clock);  /*递归深入顶点u进行DFS*/
        }
        else if (VERTEX_STAT_DISCOVERED == status(u))
        {
            /*如果顶点u的状态是发现而未处理完毕,说明在遍历树中v实际上是u的后代*/
            type(v, u) = EDGE_TYPE_BACKARD; /*后向边,回边*/
        }
        else
        {
            /*顶点u的状态为已处理完毕*/
            if (dTime(v) < dTime(u))
                type(v, u) = EDGE_TYPE_FORWARD; /*前向边*/
            else
                type(v, u) = EDGE_TYPE_CROSS;   /*两个顶点在遍历树中不构成祖先后代关系*/
        }
    }

    /*顶点v处理完毕,标记为visited,已经记录ftime*/
    fTime(v) = ++clock;
    status(v) = VERTEX_STAT_VISITED;

    return;
}

template < typename Tv, typename Te >
void Graph<Tv, Te>::DFS_V2(int v, int &clock)
{
    Stack<int> stack;

    status(v) = VERTEX_STAT_DISCOVERED;
    dTime(v) = ++clock;

    /*将其入栈*/
    stack.push(v);

    /*栈不为空则一直迭代*/
    while (!stack.empty())
    {
        int v = stack.top();

        bool discovered = false;

        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
        {
            if (VERTEX_STAT_UNDISCOVERED == status(u))
            {
                discovered = true;
                /*可引入一条树边*/
                type(v, u) = EDGE_TYPE_TREE;
                status(u) = VERTEX_STAT_DISCOVERED;
                parent(u) = v;
                dTime(u) = ++clock;

                /*将顶点u入栈,实则对应着递归以u开始进行DFS*/
                stack.push(u);
                /*当我们发现v存在一个未发现的顶点u,则随即开始新一轮递归DFS,因此这里发现一个undiscovered的顶点后需要跳出这轮迭代*/
                break;
            }
            else if (VERTEX_STAT_DISCOVERED == status(u))
            {
                /*回边*/
                type(v, u) = EDGE_TYPE_BACKARD;
            }
            else
            {
                if (dTime(v) < dTime(u))
                    type(v, u) = EDGE_TYPE_FORWARD;
                else
                    type(v, u) = EDGE_TYPE_CROSS;
            }
        }

        if (!discovered)
        {
            //::printf("vertex %d visited ...\n", v);
            stack.pop();
            status(v) = VERTEX_STAT_VISITED;
            fTime(v) = ++clock;
        }
    }
}

template < typename Tv, typename Te >
void Graph<Tv, Te>::dfs(int s)
{

    /*检查s的合法性*/
    if (0 < s || s >= m_vertex_cnt)
        THROW_EXCEPTION(IndexOutOfBoundException, "trying to start dfs from invalid index of vertex ...");

    int clock = 0;  /*最开始时间设置为0*/
    int v = s;

    reset();
   
    /*
    *   基于BFS接口实现能遍历以顶点v作为根节点的dfs-tree中的所有顶点,
    *   但是这颗dfs-tree不一定包含图中所有顶点
    *   因此整个图的DFS算法应当检查所有的顶点,
    *   如果发现一个顶点处于undiscovered状态,则随即以这个顶点为起点调用一次DFS接口
    *   直到所有顶点的状态都不为discovered
    *   此时认为图中所有顶点按照DFS算法顺序进行了一次且仅一次的访问
    */
    do
    {
        if (VERTEX_STAT_UNDISCOVERED == status(v))
            DFS_V2(v, clock);
    } while (s != (v = (++v) % m_vertex_cnt));  /*环形转一圈*/
}

template < typename Tv, typename Te >
void Graph<Tv, Te>::tSort(int s, Stack<int> &stack)
{
    /*检查s的合法性*/
    if (0 < s || s >= m_vertex_cnt)
        THROW_EXCEPTION(IndexOutOfBoundException, "trying to start topological sort from invalid index of vertex ...");

    int v = s;

    do
    {
        if (VERTEX_STAT_UNDISCOVERED == status(v))
            if (!TSort(v, stack))
            {
                while (!stack.empty())
                    stack.pop();
                    break;
            }
    } while (s != (v = (++v) % m_vertex_cnt));
}

template < typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, Stack<int> &stack)
{
    Stack<int> tmp;

    status(v) = VERTEX_STAT_DISCOVERED;
    tmp.push(v);

    while (!tmp.empty())
    {
        int v = tmp.top();
        bool find = false;

        for (int u = firstNbr(v); u > -1 && !find; u = nextNbr(v, u))
        {
            switch (status(u))
            {
                case VERTEX_STAT_UNDISCOVERED:
                {
                    /*未发现的则令之入栈*/ 
                    status(u) = VERTEX_STAT_DISCOVERED;
                    tmp.push(u);
                    find = true;
                    break;
                }
                case VERTEX_STAT_DISCOVERED:
                {
                    /*发现了一条回边,则原图存在环路,不满足存在拓扑排序要求*/
                    return false;
                }
                default:
                {
                    /*前向边和跨越边不处理*/
                    break;
                }
            }
        }

        if (!find)
        {
            /*当前顶点v的所有邻居都处理完,将顶点v状态设置为VISITED,同时将其入栈stack*/
            status(v) = VERTEX_STAT_VISITED;
            tmp.pop();
            stack.push(v);
        }
    }

    return true;
}

}
#endif
