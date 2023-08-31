#pragma once

#ifndef CORE_MTL_HASHTABLE_H_
#define CORE_MTL_HASHTABLE_H_

#include "schmgr/core/mtl/Vector.h"
#include <iostream>

namespace core::mtl
{

// 哈希表，利用开链法解决冲突
// 以Vector为桶
// 节点为链表
// 作为unordered_map的底层实现, 插入的元素为pair, 且不允许重复

template <class K, class V>
class HashTable
{
private:
    struct Node
    {
        std::pair<const K, V> data;
        Node* next;
        Node(const K& key, const V& value, Node* next = nullptr)
            : data(key, value), next(next) {}
    };

    Vector<Node*> _buckets;
    size_t _size;

    // 哈希函数
    size_t hash(const K& key) const
    {
        return key % _buckets.size();
    }

    // 查找节点
    Node* findNode(const K& key) const
    {
        size_t index = hash(key);
        Node* node = _buckets[index];
        while (node != nullptr && node->data.first != key)
            node = node->next;
        return node;
    }

public:
    class iterator;

public:
    // 函数声明
    HashTable();
    ~HashTable();

    V& operator[](const K& key);                        // 重载[]操作符
    V& at(const K& key);                                // 返回key对应的value, 如果key不存在则抛出异常
    size_t count(const K& key) const;                   // 返回key在哈希表中出现的次数
    size_t bucket_count() const;                        // 返回哈希表中桶的个数

    void insert(const std::pair<K, V>& p);              // 插入pair
    void erase(const K& key);                           // 删除键值对
    void clear();                                       // 清空哈希表

    size_t size() const;                                // 返回哈希表中键值对的个数
    bool empty() const;                                 // 判断哈希表是否为空

    iterator begin();                                   // 返回哈希表头部迭代器
    iterator end();                                     // 返回哈希表尾部迭代器
    iterator find(const K& key);                        // 返回指向key的迭代器

    void rehash(size_t n);                              // 重新设置哈希表的桶的个数, 并重新哈希

    void print();                                       // 打印哈希表(测试用)
};

template <class K, class V>
class HashTable<K, V>::iterator
{
private:
    typename Vector<Node*>::iterator it;

public:
    iterator() {}
    iterator(const typename Vector<Node*>::iterator& it) : it(it) {}
    ~iterator() {}

    // iterator &operator++()
    // {
    // // 当前桶链表的遍历：迭代器首先遍历当前桶中的链表或红黑树，访问该桶中的每个节点。在这个过程中，迭代器可能会递增并移动到链表中的下一个节点。
    // // 当前桶遍历完成：当迭代器遍历完当前桶的链表或红黑树后，如果还没有遍历到end()，则迭代器会移动到下一个非空桶。
    // // 寻找下一个非空桶：迭代器在哈希表的桶中依次查找下一个非空的桶。这通常是通过跳过空桶来实现的，直到找到一个非空桶或者遍历到了哈希表的末尾。
    //     if (it != nullptr && (*it)->next != nullptr) {
    //         it = (*it)->next;
    //         return *this;
    //     } else {
    //         ++it;
    //         while (*it == nullptr && it != _buckets.end())
    //             ++it;
    //         return *this;
    //     }
    // }
    // iterator operator++(int)
    // {
    //     iterator tmp = *this;
    //     if (it != nullptr && (*it)->next != nullptr) {
    //         it = (*it)->next;
    //         return tmp;
    //     } else {
    //         ++it;
    //         while (*it == nullptr && it != _buckets.end())
    //             ++it;
    //         return tmp;
    //     }
    // }

    std::pair<const K, V>& operator*() const
    {
        return (*it)->data;
    }
    std::pair<const K, V>* operator->() const
    {
        return &((*it)->data);
    }

    bool operator==(const iterator& other) const
    {
        return it == other.it;
    }
    bool operator!=(const iterator& other) const
    {
        return it != other.it;
    }
};

template <class K, class V>
HashTable<K, V>::HashTable()
{
    _size = 0;
    _buckets.resize(16);
    for (size_t i = 0; i < _buckets.size(); i++)
        _buckets[i] = nullptr;
}

template <class K, class V>
HashTable<K, V>::~HashTable()
{
    clear();
}

template <class K, class V>
V& HashTable<K, V>::operator[](const K& key)
{
    Node* node = findNode(key);
    if (node == nullptr)
    {
        insert(std::pair<K, V>(key, V()));
        node = findNode(key);
    }
    return node->data.second;
}

template <class K, class V>
V& HashTable<K, V>::at(const K& key)
{
    Node* node = findNode(key);
    if (node == nullptr)
        throw std::out_of_range("key not found");
    return node->data.second;
}

template <class K, class V>
size_t HashTable<K, V>::count(const K& key) const
{
    Node* node = findNode(key);
    if (node == nullptr)
        return 0;
    else
        return 1;
}

template <class K, class V>
size_t HashTable<K, V>::bucket_count() const
{
    return _buckets.size();
}

template <class K, class V>
void HashTable<K, V>::insert(const std::pair<K, V>& p)
{
    size_t index = hash(p.first);
    Node* node = _buckets[index];
    while (node != nullptr && node->data.first != p.first)
        node = node->next;
    if (node == nullptr)
    {
        node = new Node(p.first, p.second, _buckets[index]);
        _buckets[index] = node;
        _size++;
    }
}

template <class K, class V>
void HashTable<K, V>::erase(const K& key)
{
    size_t index = hash(key);
    Node* node = _buckets[index];
    Node* prev = nullptr;
    while (node != nullptr && node->data.first != key)
    {
        prev = node;
        node = node->next;
    }
    if (node != nullptr)
    {
        if (prev == nullptr)
            _buckets[index] = node->next;
        else
            prev->next = node->next;
        delete node;
        _size--;
    }
}

template <class K, class V>
void HashTable<K, V>::clear()
{
    for (size_t i = 0; i < _buckets.size(); i++)
    {
        Node* node = _buckets[i];
        while (node != nullptr)
        {
            Node* next = node->next;
            delete node;
            node = next;
        }
        _buckets[i] = nullptr;
    }
    _size = 0;
}

template <class K, class V>
size_t HashTable<K, V>::size() const
{
    return _size;
}

template <class K, class V>
bool HashTable<K, V>::empty() const
{
    return _size == 0;
}

// template <class K, class V>
// typename HashTable<K, V>::iterator HashTable<K, V>::begin()
// {
//     typename Vector<Node *>::iterator it = _buckets.begin();
//     while (*it == nullptr && it != _buckets.end())
//         it++;
//     return iterator(it);
// }

// template <class K, class V>
// typename HashTable<K, V>::iterator HashTable<K, V>::end()
// {
//     return iterator(_buckets.end());
// }

// template <class K, class V>
// typename HashTable<K, V>::iterator HashTable<K, V>::find(const K &key)
// {
//     size_t index = hash(key);
//     Node *node = _buckets[index];
//     while (node != nullptr && node->data.first != key)
//         node = node->next;
//     return iterator(typename Vector<Node *>::iterator(node));
// }

template <class K, class V>
void HashTable<K, V>::rehash(size_t n)
{
    Vector<Node*> newBuckets(n);
    for (size_t i = 0; i < newBuckets.size(); i++)
        newBuckets[i] = nullptr;
    for (size_t i = 0; i < _buckets.size(); i++)
    {
        Node* node = _buckets[i];
        while (node != nullptr)
        {
            Node* next = node->next;
            size_t index = node->data.first % newBuckets.size();
            node->next = newBuckets[index];
            newBuckets[index] = node;
            node = next;
        }
    }
    _buckets = newBuckets;
}

template <class K, class V>
void HashTable<K, V>::print()
{
    for (size_t i = 0; i < _buckets.size(); i++)
    {
        Node* node = _buckets[i];
        while (node != nullptr)
        {
            std::cout << node->data.first << ":" << node->data.sceond << " ";
            node = node->next;
        }
    }
    std::cout << std::endl;
}

}

#endif // CORE_MTL_HASHTABLE_H_