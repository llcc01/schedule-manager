#pragma once

#ifndef CORE_MTL_MAP_H_
#define CORE_MTL_MAP_H_

#include "schmgr/core/mtl/RBTree.h"
#include <iostream>
#include <initializer_list>

namespace core::mtl
{



template <class K, class V>
class Map {
private:
    RBtree<K, V> tree;

public:
    class iterator;

public:
    Map() {}
    ~Map() {}
    Map(std::initializer_list<std::pair<K, V>> l);

    void insert(const K& key, const V& value);
    void insert(const std::pair<K, V>& p);    
    void erase(const K& key);                 
    void clear();                             

    bool empty();                             
    size_t size();                            

    V& operator[](const K& key);              

    iterator begin() const;                   
    iterator end() const;                     
    iterator find(const K& key) const;        
    iterator lower_bound(const K& key);       
    iterator upper_bound(const K& key);       

    void print();                             
};

template <class K, class V>
class Map<K, V>::iterator {
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
    bool operator==(const iterator& rhs) const {
        return it == rhs.it;
    }
    bool operator!=(const iterator& rhs) const {
        return it != rhs.it;
    }

    std::pair<K, V>& operator*() {
        return it.operator*();
    }

    std::pair<K, V>* operator->() {
        return it.operator->();
    }

};

template <class K, class V>
Map<K, V>::Map(std::initializer_list<std::pair<K, V>> l) {
    for (auto &p : l) {
        tree.insert(p.first, p.second);
    }
}

template <class K, class V>
void Map<K, V>::insert(const K& key, const V& value) {
    tree.insert(key, value);
}

template <class K, class V>
void Map<K, V>::insert(const std::pair<K, V>& p) {
    tree.insert(p.first, p.second);
}


template <class K, class V>
void Map<K, V>::erase(const K& key) {
    tree.erase(key);
}

template <class K, class V>
void Map<K, V>::clear() {
    tree.clear();
}

template <class K, class V>
bool Map<K, V>::empty() {
    return tree.empty();
}

template <class K, class V>
size_t Map<K, V>::size() {
    return tree.size();
}

template <class K, class V>
V& Map<K, V>::operator[](const K& key) {
    return tree[key];
}

template <class K, class V>
typename Map<K, V>::iterator Map<K, V>::begin() const {
    return iterator(tree.begin());
}

template <class K, class V>
typename Map<K, V>::iterator Map<K, V>::end() const {
    return iterator(tree.end());
}

template <class K, class V>
typename Map<K, V>::iterator Map<K, V>::find(const K& key) const {
    return iterator(tree.find(key));
}

template <class K, class V>
typename Map<K, V>::iterator Map<K, V>::lower_bound(const K& key) {
    return iterator(tree.lower_bound(key));
}

template <class K, class V>
typename Map<K, V>::iterator Map<K, V>::upper_bound(const K& key) {
    return iterator(tree.upper_bound(key));
}

template <class K, class V>
void Map<K, V>::print() {
    std::cout << "Map:";
    tree.print();
    std::cout << std::endl;
}

}

#endif // CORE_MTL_MAP_H_