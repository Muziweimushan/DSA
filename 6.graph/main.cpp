#include <iostream>
#include <cstdio>
#include <stdint.h>
#include "ListGraph.h"

using namespace MyLib;

int main(void)
{
    MyLib::ListGraph<char, int> graph;

    graph.insert('s');  /*0*/
    graph.insert('a');  /*1*/
    graph.insert('b');  /*2*/
    graph.insert('c');  /*3*/
    graph.insert('d');  /*4*/
    graph.insert('e');  /*5*/
    graph.insert('f');  /*6*/
    graph.insert('g');  /*7*/

    graph.insert(100, 0, 1, 10);
    graph.insert(100, 0, 3, 10);
    graph.insert(100, 0, 4, 10);
    graph.insert(100, 3, 2, 10);
    graph.insert(100, 1, 5, 10);
    graph.insert(100, 5, 6, 10);
    graph.insert(100, 5, 7, 10);

    
    graph.bfs(0);
#if 0
    MyLib::ListGraph<int, int> graph;


    (void)graph.insert(10);
    (void)graph.insert(11);
    (void)graph.insert(12);
    (void)graph.insert(13);
    (void)graph.insert(14);
    
    try
    {  
        graph.insert(100, 0, 1, 10);
        graph.insert(100, 0, 2, 10);
        graph.insert(100, 1, 2, 10);
        graph.insert(100, 0, 3, 10);
        graph.insert(100, 3, 4, 10);
        graph.insert(100, 4, 0, 10);
    }
    catch (MyLib::Exception &exception)
    {
        ::std::cout << "exception : " << exception.location() << "\n" << exception.message() << ::std::endl;
    }
    catch (...)
    {
        ::std::cout << "failed ..." << ::std::endl;
    }
#endif
    ::printf("\n\n\n\n\n\n\n\n\n\nstart dfs ...\n\n\n\n\n\n");

    ::printf("vertex cnt = %d\n", graph.getVertexCnt());
    ::printf("edge cnt = %d\n", graph.getEdgeCnt());

    graph.dfs(0);

    for (int i = 0; i < graph.getVertexCnt(); i++)
    {
        ::printf("vertex[%d] dtime = %d, ftime = %d\n", i, graph.dTime(i), graph.fTime(i));
    }

    return 0;
}
