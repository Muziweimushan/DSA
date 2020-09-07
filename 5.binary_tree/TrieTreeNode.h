/*
*
*   Trie树的简单实现
*       trie树(键树),是有根有序树,其中的每个节点均包含r个分支(在下面使用charset_num)
*       深度为d的节点分别对应于长度为d的字符串,且祖先所对应字符串必为后代所对应字符串的前缀
*
*
*/


#pragma once

#ifndef __TRIETREENODE_H__
#define __TRIETREENODE_H__

#include <cstring>
#include "Object.h"

namespace MyLib
{

enum { TRIETREE_CHARSET_NUM = 256 };    /*字符集个数*/

#define TrieTreePosi TrieTreeNode *

typedef enum
{
    TRIETREE_NODE_COLOR_RED = 0x0,
    TRIETREE_NODE_COLOR_BLACK = 0x1,
    TRIETREE_NODE_COLOR_CNT
}TRIETREE_NODE_COLOR_E;

/*trie-tree节点定义*/
struct TrieTreeNode
{
    TRIETREE_NODE_COLOR_E m_color;
    TrieTreePosi m_child[TRIETREE_CHARSET_NUM];
};

class TrieTree : public Object
{
public:
    TrieTree();
    /*添加新key*/
    void insert(const char *key);
    /*删除key*/
    void remove(const char *key);
    /*判空*/
    bool empty() const;
    /*清空*/
    void clean();
    /*查找操作*/
    bool find(const char *key) const;

    virtual ~TrieTree();
private:
    TrieTreeNode m_root; 

    void insert(const char *key, int key_len, int d, TrieTreePosi &pos);
    void initTreeNode(TrieTreePosi node);
    void clean(TrieTreePosi &pos);
    void remove(const char *key, const int key_len, int d, TrieTreePosi &pos);
    /*功能函数:在树中查找key,当前深度为depth,对应的树节点为pos,如查找到了,则返回最底下的节点的位置,找不到返回空*/
    TrieTreePosi find(const char *key, const int key_len, int depth, TrieTreePosi pos) const;
};


}

#endif
