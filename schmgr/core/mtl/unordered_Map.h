#pragma once

#ifndef CORE_MTL_UNORDERED_MAP_H_
#define CORE_MTL_UNORDERED_MAP_H_

#include "schmgr/core/mtl/HashTable.h"
#include <iostream>

namespace core::mtl
{

// 以哈希表为底层实现的无序映射

template <class K, class V>

class unordered_Map
{
private:
    HashTable<K, V> table;

public:
    class iterator;

public:
    // 函数声明
    unordered_Map();
    unordered_Map(std::initializer_list<std::pair<K, V>>); // 初始化列表
    ~unordered_Map();

    V& operator[](const K& key);               // 重载[]操作符
    V& at(const K& key);                       // 返回key对应的value, 如果key不存在则抛出异常
    size_t count(const K& key) const;          // 返回key在哈希表中出现的次数
    size_t bucket_count() const;               // 返回哈希表中桶的个数

    void insert(const std::pair<K, V>& p);     // 插入pair
    void erase(const K& key);                  // 删除键值对
    void clear();                              // 清空哈希表

    size_t size() const;                       // 返回哈希表中键值对的个数
    bool empty() const;                        // 判断哈希表是否为空

    iterator begin();                          // 返回map头部迭代器
    iterator end();                            // 返回map尾部迭代器
    iterator find(const K& key);               // 返回指向key的迭代器

    void print();                              // 打印map(测试用)
};

template <class K, class V>
class unordered_Map<K, V>::iterator
{
private:
    typename HashTable<K, V>::iterator it;

public:
    iterator() {}
    iterator(const typename HashTable<K, V>::iterator& it) : it(it) {}
    ~iterator() {}

    // iterator &operator++()
    // {
    //     ++it;
    //     return *this;
    // }
    // iterator operator++(int)
    // {
    //     iterator tmp = *this;
    //     ++it;
    //     return tmp;
    // }
    // iterator &operator--()
    // {
    //     --it;
    //     return *this;
    // }
    // iterator operator--(int)
    // {
    //     iterator tmp = *this;
    //     --it;
    //     return tmp;
    // }

    std::pair<const K, V>& operator*()
    {
        return *it;
    }
    std::pair<const K, V>* operator->()
    {
        return &(*it);
    }

    bool operator==(const iterator& other)
    {
        return it == other.it;
    }
    bool operator!=(const iterator& other)
    {
        return it != other.it;
    }
};

template <class K, class V>
unordered_Map<K, V>::unordered_Map()
{
}

template <class K, class V>
unordered_Map<K, V>::unordered_Map(std::initializer_list<std::pair<K, V>> l)
{
    for (auto it = l.begin();it != l.end();++it)
    {
        table.insert(*it);
    }
}

template <class K, class V>
unordered_Map<K, V>::~unordered_Map()
{
}

template <class K, class V>
V& unordered_Map<K, V>::operator[](const K& key)
{
    return table[key];
}

template <class K, class V>
V& unordered_Map<K, V>::at(const K& key)
{
    return table.at(key);
}

template <class K, class V>
size_t unordered_Map<K, V>::count(const K& key) const
{
    return table.count(key);
}

template <class K, class V>
size_t unordered_Map<K, V>::bucket_count() const
{
    return table.bucket_count();
}

template <class K, class V>
void unordered_Map<K, V>::insert(const std::pair<K, V>& p)
{
    table.insert(p);
}

template <class K, class V>
void unordered_Map<K, V>::erase(const K& key)
{
    table.erase(key);
}

template <class K, class V>
void unordered_Map<K, V>::clear()
{
    table.clear();
}

template <class K, class V>
size_t unordered_Map<K, V>::size() const
{
    return table.size();
}

template <class K, class V>
bool unordered_Map<K, V>::empty() const
{
    return table.empty();
}

// template <class K, class V>
// typename unordered_Map<K, V>::iterator unordered_Map<K, V>::begin()
// {
//     return iterator(table.begin());
// }

// template <class K, class V>
// typename unordered_Map<K, V>::iterator unordered_Map<K, V>::end()
// {
//     return iterator(table.end());
// }

// template <class K, class V>
// typename unordered_Map<K, V>::iterator unordered_Map<K, V>::find(const K &key)
// {
//     return iterator(table.find(key));
// }

template <class K, class V>
void unordered_Map<K, V>::print()
{
    table.print();
}

}

#endif // CORE_MTL_UNORDERED_MAP_H_