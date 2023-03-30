/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 15:11:08
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-20 21:42:30
 * @FilePath: /cpptest/skiplistpro/myRedis_server/node.h
 */
#ifndef NODE_H
#define NODE_H
#include <vector>
using std::vector;

template<typename K, typename V>
class Node
{

public:
    Node() = default;
    Node(K k, V v, int level);
    ~Node();

    K get_key()const;
    V get_val()const;
    void set_val(V val);

    vector<Node*>next;



private:

    K key;
    V val;
    int node_level;

};


template<typename K, typename V>
Node<K,V> :: Node(K k, V v, int level)
{
    key = k;
    val = v;
    node_level = level;
    next.resize(node_level, nullptr);
}

template<typename K, typename V>
Node<K,V>::~Node()
{
    
}

template<typename K, typename V>
K Node<K,V> :: get_key()const
{
    return key;
}

template<typename K, typename V>
V Node<K,V>::get_val()const
{
    return val;
}

template<typename K, typename V>
void Node<K,V>::set_val(V v)
{
    val = v;
}

#endif 
