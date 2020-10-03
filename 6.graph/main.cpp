#include <iostream>
#include <cstdio>
#include <stdint.h>
#include "ListGraph.h"

using namespace MyLib;

int main(void)
{
    MyLib::ListGraph<char, int> graph;

    graph.insert('a');  /*0*/
    graph.insert('b');  /*1*/
    graph.insert('c');  /*2*/
    graph.insert('f');  /*3*/
    graph.insert('g');  /*4*/

    graph.insert(100, 0, 1, 10);
    graph.insert(100, 1, 2, 10);
    graph.insert(100, 0, 2, 10);
    graph.insert(100, 0, 3, 10);
    graph.insert(100, 3, 4, 10);
    graph.insert(100, 4, 0, 10);
    graph.insert(100, 4, 2, 10);

    ::printf("\n\n\n\n\n\n\n\n\n\nstart dfs ...\n\n\n\n\n\n");

    ::printf("vertex cnt = %d\n", graph.getVertexCnt());
    ::printf("edge cnt = %d\n", graph.getEdgeCnt());

    try
    {
    
        graph.dfs(0);
    }
    catch (const MyLib::Exception &exception)
    {
        ::printf("failed: %s:%s\n", exception.location(), exception.message());
    }


    ::printf("endof dfs ...\n");

    for (int i = 0; i < graph.getVertexCnt(); i++)
    {
        ::printf("vertex[%d] dtime = %d, ftime = %d\n", i, graph.dTime(i), graph.fTime(i));
    }

    return 0;
}
