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
    (void)graph.insert(15);

    graph.insert(0, 0, 1, 10);
    graph.insert(1, 3, 4, 10);
    graph.insert(2, 2, 3, 10);
    return 0;
}
