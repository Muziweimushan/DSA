#include <iostream>
#include "DTLib/DTString.h"
#include "DTLib/StaticArray.h"
#include "DTLib/DynamicArray.h"
#include "DTLib/DynamicList.h"
#include "DTLib/LinkList.h"
#include "DTLib/LinkStack.h"
#include "DTLib/LinkQueue.h"
#include "DTLib/BTree.h"
#include "DTLib/ListGraph.h"
#include "DTLib/SmartPointer.h"
#include "DTLib/SharedPointer.h"
#include "DTLib/Sort.h"

#include "DTLib/MatrixGraph.h"

using namespace DTLib;
using namespace std;

void ArrayDemo()
{
    cout << "ArrayDemo(): " << endl;

    double arr[] = { 0.5, 0.2, 0.3, 0.4, 0.1 };
    int len = sizeof(arr)/sizeof(*arr);

    StaticArray<double, 10> sa;
    DynamicArray<double> da;

    da.resize(len);

    for(int i=0; i<len; i++)
    {
        sa[i] = arr[i];
        da[i] = arr[i];
    }

    cout << "StaticArray: " << endl;

    for(int i=0; i<sa.length(); i++)
    {
        cout << sa[i] << endl;
    }

    cout << "DynamicArray: " << endl;

    for(int i=0; i<da.length(); i++)
    {
        cout << da[i] << endl;
    }
}

void ListDemo()
{
    cout << "ListDemo(): " << endl;

    /*
     * 线性表List:有限个相同类型元素的集合,这些元素在逻辑上有序是排列的
     * List提供插入(insert) 删除(remove) 获取/设置某位置上元素的值(get/set) 查找(find) 获取长度(length) 清空(claer) 操作
     *
     * List首先分两大类: SeqList和LinkList,SeqList指数据元素在物理上也是连续排列的,而LinkList也就是链表在物理上一般不是连续排列的,为此链表元素本身需要额外信息存放与之相邻元素的信息(一般就是一个指针指向下一个,也就是LinkList单链表)
     *
     * SeqList(顺序存储的线性表)有两种:StaticList和DynamicList,区别就是内部存放元素的空间是否是动态申请的,StaticList直接使用语言本身的数组作为元素实际的存放地,而DynamicList则是在初始化时通过动态内存申请分配一段空间存放数据元素,因此SeqList的容量(capacity)是固定的,而DynamicList的容量则是可以改变的(通过resize接口),这是两者的最主要区别,在构造这两个SeqList时都需要提供初始容量,可通过capacity接口获取;
     *
     * 链表则是分为单链表(LinkList) 单向循环链表(CircleLinkList,最后一个元素指向第一个元素) 双向链表(每个元素维护两个引用,一个指向它的前驱,一个指向它的后继) 双向循环链表(DualCircleList,把单向循环链表变成双向即可)
     */

    LinkList<double> list;
    DynamicList<double> dl(50);

    for(int i=0; i<10; i++)
    {
        dl.insert(i/10.0);
    }

    dl.remove(5);
    
    for(int i=0; i<dl.length(); i++)
    {
        list.insert(dl[i]);
    }

    /*因为链表在物理上不连续,因此链表的随机访问性能比较弱鸡,每次都要从头开始找,所以借助游标手段实现O(1)的遍历,方法就是通过move指定从哪里开始,通过next指向下一个元素,通过current访问当前元素,通过end判断是否遍历已经结束,类似与STL的iterator*/
    for(list.move(0); !list.end(); list.next())
    {
        cout << list.current() << endl;
    }
}

void StackQueueDemo()
{
    cout << "StackQueueDemo(): " << endl;

    /* 栈和队列也属于线性数据结构,它们的特点是插入或删除只能在固定线性容器的一端进行操作
     * 栈的特性是后进先出,即入栈(push)和出栈(pop)只能在栈顶一侧进行
     * 队列Queue的入队列(add)则是固定从队列的一端进行插入,而出对列(remove)则是从另一端进行,也就是满足先进先出的特性
     *
     * 此外Queue和Stack都提供访问下一个出栈/下一个出队列元素的操作,即top和front接口
     *
     * Stack和Queue内部可以组合使用List或原生数组作为存放数据的实际内存空间,因为LinkList的容量是不固定的,因此使用LinkList的Stack/Queue(LinkStack/LinkQueue)允许插入不定量的数据元素,对应的StaticStack/StackQueue则需要在模板参数中指定容量大小
     *
     * */
    LinkStack<int> stack;
    LinkQueue<int> queue;

    for(int i=0; i<10; i++)
    {
        stack.push(i);
        queue.add(i);
    }

    cout << "Stack: " << endl;

    while( stack.size() )
    {
        cout << stack.top() << endl;

        stack.pop();
    }

    cout << "Queue: " << endl;

    while( queue.length() )
    {
        cout << queue.front() << endl;

        queue.remove();
    }
}

void BinTreeDemo()
{
    cout << "BinTreeDemo(): " << endl;

    /*二叉树数据结构提供插入(insert) 删除(remove) 查找(find) 还提供二叉树的4种遍历接口 提供获取树的度(degree) 高度(height) 节点个数(count)接口*/

    BTree<int> bt; // empty tree
    BTreeNode<int>* node = NULL;
    
    // construct binary tree
    bt.insert(1, NULL); // 1 is tree root

    node = bt.root();
    bt.insert(2, node); // 2 is left child of 1
    bt.insert(3, node); // 3 is right child of 1

    node = bt.find(2);
    bt.insert(4, node, RIGHT); // 4 is right child of 2
    bt.insert(5, node, LEFT);  // 5 is left child of 2

    node = bt.find(3);
    bt.insert(6, node, RIGHT); // 6 is right child of 3

    cout << "After construct: " << endl;
    cout << "Height: " << bt.height() << endl;
    cout << "Count: " << bt.count() << endl;
    cout << "Degree: " << bt.degree() << endl;

    cout << "BFS to access node value: " << endl;

    for(bt.begin(); !bt.end(); bt.next())
    {
        cout << bt.current() << endl;
    }

    bt.remove(2); // remove sub tree from node 2

    cout << "After remove: " << endl;
    cout << "Height: " << bt.height() << endl;
    cout << "Count: " << bt.count() << endl;
    cout << "Degree: " << bt.degree() << endl;

    cout << "BFS to access node value: " << endl;

    /*同样提供与List类似的遍历操作接口,此处以层次遍历的方式进行树节点的遍历,也就是图的广度优先搜索*/
    for(bt.begin(); !bt.end(); bt.next())
    {
        cout << bt.current() << endl;
    }
}

void GraphDemo()
{
    cout << "GraphDemo(): " << endl;

    ListGraph<char, float> graph;

    // construct graph
    graph.addVertex('A');  // 0
    graph.addVertex('B');  // 1
    graph.addVertex('C');  // 2 
    graph.addVertex('D');  // 3
    graph.addVertex('E');  // 4

    graph.setEdge(0, 1, 0.2);  // v0->v1, weight: 0.2
    graph.setEdge(0, 4, 1.0);  // v0->v4, weight: 1.0
    graph.setEdge(1, 2, 0.3);  // v1->v2, weight: 0.3
    graph.setEdge(1, 4, 0.7);  // v1->v4, weight: 0.7
    graph.setEdge(2, 0, 0.4);  // v2->v0, weight: 0.4
    graph.setEdge(2, 3, 0.4);  // v2->v3, weight: 0.4
    graph.setEdge(2, 4, 0.3);  // v2->v4, weight: 0.3
    graph.setEdge(3, 4, 0.5);  // v3->v4, weight: 0.5
    graph.setEdge(4, 2, 0.2);  // v4->v2, weight: 0.2

    SharedPointer< Array<int> > r = graph.dijkstra(0, 4, 0xFFFF);
    double d = 0;

    cout << graph.getVertex((*r)[0]);

    for(int i=1; i<r->length(); i++)
    {
        d += graph.getEdge((*r)[i-1], (*r)[i]);
        cout << " -> " << graph.getVertex((*r)[i]);
    }

    cout << endl;
    cout << "distance: " << d << endl;
}

void MatrixGraphDemo()
{
    cout << "MatrixGraphDemo(): " << endl;

    //ListGraph<char, float> graph;
    MatrixGraph<5, char, float> graph;

    // construct graph
    //graph.addVertex('A');  // 0
    //graph.addVertex('B');  // 1
    //graph.addVertex('C');  // 2 
    //graph.addVertex('D');  // 3
    //graph.addVertex('E');  // 4
    //
    graph.setVertex(0, 'A');
    graph.setVertex(1, 'B');
    graph.setVertex(2, 'C');
    graph.setVertex(3, 'D');
    graph.setVertex(4, 'E');

    graph.setEdge(0, 1, 0.2);  // v0->v1, weight: 0.2
    graph.setEdge(0, 4, 1.0);  // v0->v4, weight: 1.0
    graph.setEdge(1, 2, 0.3);  // v1->v2, weight: 0.3
    graph.setEdge(1, 4, 0.7);  // v1->v4, weight: 0.7
    graph.setEdge(2, 0, 0.4);  // v2->v0, weight: 0.4
    graph.setEdge(2, 3, 0.4);  // v2->v3, weight: 0.4
    graph.setEdge(2, 4, 0.3);  // v2->v4, weight: 0.3
    graph.setEdge(3, 4, 0.5);  // v3->v4, weight: 0.5
    graph.setEdge(4, 2, 0.2);  // v4->v2, weight: 0.2

    SharedPointer< Array<int> > r = graph.dijkstra(0, 4, 0xFFFF);
    double d = 0;

    cout << graph.getVertex((*r)[0]);

    for(int i=1; i<r->length(); i++)
    {
        d += graph.getEdge((*r)[i-1], (*r)[i]);
        cout << " -> " << graph.getVertex((*r)[i]);
    }

    cout << endl;
    cout << "distance: " << d << endl;
}

void SortDemo()
{
    cout << "SortDemo(): " << endl;

    int arr[] = {3, 1, 2, 5, 4, 6};
    int len = sizeof(arr)/sizeof(*arr);

    cout << "min -> max(Bubble sort): " << endl;

    Sort::Bubble(arr, len);     /*冒泡排序*/

    for(int i=0; i<len; i++)
    {
        cout << arr[i] << endl;
    }

    cout << "max -> min(Quick sort): " << endl;

    Sort::Quick(arr, len, false);   /*快速排序*/

    for(int i=0; i<len; i++)
    {
        cout << arr[i] << endl;
    }
    
    /*除此之外还支持归并排序(Merge) 选择排序(Select) 插入排序(Inser) 希尔排序(Shell)*/
}

void PointerDemo()
{
    cout << "PointerDemo(): " << endl;

    SmartPointer<int> p1;
    SmartPointer<int> p2;

    p1 = new int(1);
    p2 = p1;

    cout << "p1: " << (!p1.isNull() ? *p1 : -1) << endl;
    cout << "p2: " << (!p2.isNull() ? *p2 : -1) << endl;

    SharedPointer<int> pp1;
    SharedPointer<int> pp2;

    pp1 = new int(2);
    pp2 = pp1;

    cout << "pp1: " << (!pp1.isNull() ? *pp1 : -1) << endl;
    cout << "pp2: " << (!pp2.isNull() ? *pp2 : -1) << endl;

    /*SharedPointer还支持相等比较,比较两智能指针是否指向同一片内存空间*/
    SharedPointer<int> pp3 = new int(2);
    cout << (pp1 == pp2) << endl;
    cout << (pp3 == pp2) << endl;
}

void StringDemo()
{
    const char* s = "DTLib";
    String hello = "Hello: ";
    String str;

    cout << "start String demo ..." << endl;

    /*insert接口实现将入参子字符串插入字符串第i位置上,同时支持插入尾部操作,不需要提供i*/
    str.insert(hello);
    
    str += s;   /*字符串拼接*/

    cout << str.str() << endl;  /*str接口返回与C兼容的裸字符串指针*/

    int index = str.indexOf(s);     /*indexOf字串查找,如果存在返回子串下标*/

    cout << "sub string index: " <<  index << endl;
    cout << "sub string: " << str.sub(index).str() << endl;

    str.replace(s, "Delphi Tang");  /*replace字符串替换*/

    cout << str.str() << endl;

    /*startWith和endOf接口分别实现检查String对象是否以入参字符串开头/结尾*/
    cout << str.startWith(hello) << endl;
    cout << str.endOf("Tang") << endl;
    cout << str.endOf(String("Hello")) << endl;

    /*trim接口将字符串开头和结尾的空格去掉,中间的空格不会去掉*/
    String test1 = "   test1  2  3   ";
    cout << test1.trim().str() << endl;

    /*remove接口即删除操作,提供两种方式,一种指定下标和删除长度,即从index开始一共删除length个字符,另一种方式删除子串*/
    String test2 = "Just for test ...";
    cout << test2.remove(0, 5).str() << endl;
    cout << test2.remove("for ").str() << endl;
    cout << test2.remove(String("aaa")).str() << endl;

    cout << "end String demo ..." << endl;

    /*还有[]重载即访问下标为index的字符,还能做大小比较\相等\不等比较(调用C库的strcmp)*/
}



int main()
{
    //StringDemo();
    //PointerDemo();
    //SortDemo();
    //ArrayDemo();
    //ListDemo();
    //StackQueueDemo();
    //BinTreeDemo();
    //GraphDemo();
    //
    MatrixGraphDemo();

    return 0;
}
