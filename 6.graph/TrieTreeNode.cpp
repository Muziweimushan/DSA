#include <cstdlib>

#include "TrieTreeNode.h"
#include "Exception.h"

using namespace MyLib;

TrieTree::TrieTree()
{
    initTreeNode(&m_root);
}

TrieTree::~TrieTree()
{
    //::std::cout << "~TrieTree()" << ::std::endl;
    clean();    /*调用清空trie树接口即可*/
}

bool TrieTree::empty(void) const
{
    bool ret = true;
    for (int i = 0; i < TRIETREE_CHARSET_NUM; i++)
    {
        if (NULL != m_root.m_child[i])
        {
            ret = false;
            break;
        }
    }

    return ret;
}

TrieTreePosi TrieTree::find(const char *key, const int key_len, int depth, TrieTreePosi pos) const
{
    TrieTreePosi ret = NULL;

    /*如果当前树节点为空,意味着找不到,返回空*/
    if (NULL == pos)
        return ret;

    /*如果深度等于key的长度,说明找到啦*/
    if (key_len == depth)
    {
        ret = pos;
    }
    else
    {
        /*否则就继续向下*/
        int idx = key[depth];
        ret = find(key, key_len, depth + 1, pos->m_child[idx]);
    }

    return ret;
}

bool TrieTree::find(const char *key) const
{
    bool ret = true;
    int key_len = 0;

    if (NULL == key)
        THROW_EXCEPTION(InvalidParameterException, "Trie-tree : find key with NULL ptr ...");  /*合法性检查*/

    key_len = strlen(key);
    if (key_len > 0)
    {
        int idx = key[0];
        TrieTreePosi pos = find(key, key_len, 1, m_root.m_child[idx]);
        if (NULL == pos)
            ret = false;    /*树中不存在其上的值为key的一条路径*/
        else if (TRIETREE_NODE_COLOR_BLACK == pos->m_color)
            ret = false;    /*存在这样一条路径,但是最底下节点不是一个结束节点*/
    }
    else
    {
        ret = false;    /*TODO:查找的key是个空串应该是找到还是没找到呢...*/
    }

    return ret;
}

void TrieTree::clean(void)
{
    for (int i = 0; i < TRIETREE_CHARSET_NUM; i++)
        clean(m_root.m_child[i]);
}

void TrieTree::remove(const char *key, const int key_len, int d, TrieTreePosi &pos)
{
    if (NULL == pos)
        return; /*对应的key在树中并不存在...*/

    if (key_len == d)
    {
        /*抵达最底层*/
        pos->m_color = TRIETREE_NODE_COLOR_BLACK;   /*颜色改为黑色*/
    }
    else
    {
        int idx = key[d];
        /*递归检查下一层*/
        remove(key, key_len, d + 1, pos->m_child[idx]);
    }

    /*最后检查一下,当前节点是否可以删除*/
    /*如果当前节点就是一个可独立存在的节点,即它本身对应字典中一个字符串,且当前节点对应字符串的最后一个字符,则不能删除*/
    if (TRIETREE_NODE_COLOR_RED == pos->m_color)
        return;

    for (int i = 0; i < TRIETREE_CHARSET_NUM; i++)
        if (NULL != pos->m_child[i])
            return;
    
    ::free(pos);
    pos = NULL;

    return;
}

void TrieTree::remove(const char *key)
{
    if (NULL != key)
    {
        int key_len = strlen(key);
        if (key_len > 0)
        {
            int idx = key[0];
            remove(key, key_len, 1, m_root.m_child[idx]);
        }
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "ptr of key is NULL ptr ...");
    }
}

void TrieTree::initTreeNode(TrieTreePosi node)
{
    node->m_color = TRIETREE_NODE_COLOR_BLACK;
    for (int i = 0; i < TRIETREE_CHARSET_NUM; i++)
        node->m_child[i] = NULL;
}

void TrieTree::insert(const char *key, int key_len, int d, TrieTreePosi &pos)
{
    if (NULL == pos)
    {
        /*如果对应的trie树节点还未分配空间,则创建一个先*/
        pos = static_cast<TrieTreePosi>(::malloc(sizeof(TrieTreeNode)));
        if (NULL == pos)
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new trie-tree node ...");
        }
        //::std::cout << "new tree node ..." << ::std::endl;
        initTreeNode(pos);
    }

    if (key_len == d)
    {
        /*说明是最底的树节点了,该树节点的颜色为红色*/
        pos->m_color = TRIETREE_NODE_COLOR_RED;
        /*而且递归结束*/
    }
    else
    {

        /*否则递归到下一层*/
        int next_level = d + 1;
        int idx = key[d];
        //::std::cout << "idx = " << idx << ::std::endl;
        insert(key, key_len, next_level, pos->m_child[idx]);
    }
}

void TrieTree::insert(const char *key)
{
    if (NULL != key)
    {
        int str_len = ::strlen(key);
        if (str_len > 0)
        {
            int idx = key[0];
            insert(key, str_len, 1, m_root.m_child[idx]);
        }
        /*插入字符串是空串,啥都不用做*/
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "Could not insert NULL string in a trie-tree ...");
    }
}

void TrieTree::clean(TrieTreePosi &pos)
{
    if (NULL != pos)
    {
        for (int i = 0; i < TRIETREE_CHARSET_NUM; i++)
            clean(pos->m_child[i]);

        ::free(pos);  /*当前树节点空间释放*/
        pos = NULL;
    }
}