#pragma once

#ifndef CORE_MTL_MULTIMAP_H_
#define CORE_MTL_MULTIMAP_H_

#include "schmgr/core/mtl/RBTree.h"
#include <initializer_list>

namespace core::mtl
{

template <class K, class V>
class multiMap {
private:
    RBtree<K, V> tree;

public:
    class iterator;

public:
    multiMap() {}
    ~multiMap() {}
    multiMap(std::initializer_list<std::pair<K, V>>);

    void insert(const K& key, const V& value);      // 插入键值对
    void insert(const std::pair<K, V>& p);          // 插入pair
    void erase(const K& key);                       // 删除键值对
    void erase(const std::pair<K, V>& p);           // 删除pair
    void clear();                                   // 清空map

    bool empty();                                   // 判断是否为空
    size_t size();                                  // 返回map中键值对的个数

    iterator begin() const;                               // 返回map头部迭代器
    iterator end() const;                                 // 返回map尾部迭代器
    iterator find(const K& key);                    // 返回指向key的迭代器
    iterator lower_bound(const K& key);             // 返回指向第一个大于等于key的迭代器
    iterator upper_bound(const K& key);             // 返回指向第一个大于key的迭代器
    iterator equal_range(const K& key);             // 返回指向第一个大于等于key的迭代器

    void print();                                   // 打印multiMap(测试用)
};

template <class K, class V>
class multiMap<K, V>::iterator {
private:
    typename RBtree<K, V>::iterator it;

public:
    iterator() {}
    iterator(const typename RBtree<K, V>::iterator& it) : it(it) {}
    ~iterator() {}

    iterator& operator++() {
        ++it;
        return *this;
    }
    iterator operator++(int) {
        iterator tmp = *this;
        ++it;
        return tmp;
    }
    iterator& operator--() {
        --it;
        return *this;
    }
    iterator operator--(int) {
        iterator tmp = *this;
        --it;
        return tmp;
    }
    bool operator==(const iterator& rhs) {
        return it == rhs.it;
    }
    bool operator!=(const iterator& rhs) {
        return it != rhs.it;
    }

    std::pair<K, V>& operator*() {
        return it.operator*();
    }

    // 重载->运算符
    std::pair<K, V>* operator->() {
        return it.operator->();
    }
};

template <class K, class V>
multiMap<K, V>::multiMap(std::initializer_list<std::pair<K, V>> l) {
    for (auto& p : l) {
        tree.multi_insert(p.first, p.second);
    }
}

template <class K, class V>
void multiMap<K, V>::insert(const K& key, const V& value) {
    tree.multi_insert(key, value);
}

template <class K, class V>
void multiMap<K, V>::insert(const std::pair<K, V>& p) {
    tree.multi_insert(p.first, p.second);
}

template <class K, class V>
void multiMap<K, V>::erase(const K& key) {
    tree.multi_erase(key);
}

template <class K, class V>
void multiMap<K, V>::clear() {
    tree.clear();
}

template <class K, class V>
bool multiMap<K, V>::empty() {
    return tree.empty();
}

template <class K, class V>
size_t multiMap<K, V>::size() {
    return tree.size();
}

template <class K, class V>
typename multiMap<K, V>::iterator multiMap<K, V>::begin() const {
    return iterator(tree.begin());
}

template <class K, class V>
typename multiMap<K, V>::iterator multiMap<K, V>::end() const {
    return iterator(tree.end());
}

template <class K, class V>
typename multiMap<K, V>::iterator multiMap<K, V>::find(const K& key) {
    return iterator(tree.multi_find(key));
}

template <class K, class V>
typename multiMap<K, V>::iterator multiMap<K, V>::lower_bound(const K& key) {
    return iterator(tree.lower_bound(key));
}

template <class K, class V>
typename multiMap<K, V>::iterator multiMap<K, V>::upper_bound(const K& key) {
    return iterator(tree.upper_bound(key));
}

template <class K, class V>
typename multiMap<K, V>::iterator multiMap<K, V>::equal_range(const K& key) {
    return iterator(tree.multi_equal_range(key));
}

template <class K, class V>
void multiMap<K, V>::print() {
    tree.print();
}

} // namespace core::mtl

#endif // CORE_MTL_MULTIMAP_H_
