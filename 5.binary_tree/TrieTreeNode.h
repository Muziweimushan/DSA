#pragma once

#ifndef __TRIETREENODE_H__
#define __TRIETREENODE_H__


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
class TrieTreeNode : public Object
{
public:
    TRIETREE_NODE_COLOR_E m_color;
    TrieTreePosi m_child[TRIETREE_CHARSET_NUM];
};



}

#endif
