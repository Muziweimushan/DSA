#include <iostream>

#include "MatrixGraph.h"


int main(void)
{
    MyLib::MatrixGraph<int, int> graph;


    (void)graph.insert(10);
    (void)graph.insert(11);
    (void)graph.insert(12);
    (void)graph.insert(13);
    (void)graph.insert(14);


    graph.insert(100, 0, 1, 10);
    graph.insert(100, 0, 2, 10);
    graph.insert(100, 1, 2, 10);
    graph.insert(100, 0, 3, 10);
    graph.insert(100, 3, 4, 10);
    graph.insert(100, 4, 0, 10);

    graph.dfs(0);
    return 0;
}
