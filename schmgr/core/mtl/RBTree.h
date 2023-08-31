// 红黑树，用于map和multimap

#pragma once

#ifndef CORE_MTL_RBTREE_H_
#define CORE_MTL_RBTREE_H_

#include <iostream>

namespace core::mtl
{

enum Color {
    RED = 0,
    BLACK = 1
};

template <class K, class V>
class RBtree_node {
public:
    std::pair<K, V> data;
    RBtree_node* parent;
    RBtree_node* left;
    RBtree_node* right;
    int color;

public:
    RBtree_node() : parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
    RBtree_node(const K& key, const V& value, int color = RED) : data(key, value), parent(nullptr), left(nullptr), right(nullptr), color(color) {}
    RBtree_node(const std::pair<K, V>& p, int color = RED) : data(p), parent(nullptr), left(nullptr), right(nullptr), color(color) {}
    ~RBtree_node() {}

};

template <class K, class V>
class RBtree {
public:
    RBtree_node<K, V>* root;        // 根节点

public:
    class iterator {                // 迭代器
    public:
        RBtree_node<K, V>* node;                            // 迭代器指向的节点

    public:
        iterator() : node(nullptr) {}                       // 默认构造函数
        iterator(RBtree_node<K, V>* node) : node(node) {}   // 构造函数
        iterator(const iterator& it) : node(it.node) {}     // 拷贝构造函数
        iterator& operator=(const iterator& it) {           // 拷贝赋值运算符
            if (this != &it) {
                node = it.node;
            }
            return *this;
        }
        ~iterator() {}                                      // 析构函数

    public:
        iterator& operator++();                             // 前置++运算符
        iterator operator++(int);                           // 后置++运算符
        iterator& operator--();                             // 前置--运算符
        iterator operator--(int);                           // 后置--运算符
        bool operator==(const iterator& it) const;          // ==运算符
        bool operator!=(const iterator& it) const;          // !=运算符

        std::pair<K, V>& operator*();                        // *运算符
        std::pair<K, V>* operator->();                      // ->运算符

    };

public:
    RBtree();                                                               // 默认构造函数
    RBtree(const RBtree& tree);                                             // 拷贝构造函数
    RBtree& operator=(const RBtree& tree);                                  // 拷贝赋值运算符
    ~RBtree();                                                              // 析构函数

    // 用于map
public:
    void insert(const K& key, const V& value);                              // 插入节点
    void erase(const K& key);                                               // 删除节点

    iterator find(const K& key) const;                                            // 返回指向key的迭代器

    V& operator[](const K& key);                                            // 重载[]运算符

    // 用于multimap
public:
    void multi_insert(const K& key, const V& value);                            // 插入节点
    void multi_erase(const K& key);                                             // 删除节点

    // void multi_fix_insert(RBtree_node<K, V>* node);                             // 插入修复
    // void multi_fix_erase(RBtree_node<K, V>* node, RBtree_node<K, V>* parent);   // 删除修复

    iterator multi_find(const K& key);                                          // 返回指向key的迭代器
    std::pair<iterator, iterator> multi_equal_range(const K& key);              // 返回指向第一个key的迭代器和指向最后一个key的下一个位置的迭代器


    // map和multimap共用
public:
    RBtree_node<K, V>* next_node(RBtree_node<K, V>* node) const;            // 返回node的后继节点

    void clear();                                                           // 清空树
    void clear_recursive(RBtree_node<K, V>* node);                          // 递归清空树

    void fix_insert(RBtree_node<K, V>* node);                               // 插入修复
    void fix_erase(RBtree_node<K, V>* node, RBtree_node<K, V>* parent);     // 删除修复

    void left_rotate(RBtree_node<K, V>* node);                              // 左旋
    void right_rotate(RBtree_node<K, V>* node);                             // 右旋

    iterator begin() const;                                                 // 返回指向最小节点的迭代器
    iterator end() const;                                                   // 返回指向最大节点的迭代器
    iterator lower_bound(const K& key);                                     // 返回指向第一个大于等于key的迭代器
    iterator upper_bound(const K& key);                                     // 返回指向第一个大于key的迭代器

    bool empty();                                                           // 判断树是否为空
    size_t size();                                                          // 返回树中节点的个数

    void print();                                                           // 打印树(测试用)

};

template <class K, class V>
typename RBtree<K, V>::iterator& RBtree<K, V>::iterator::operator++() {
    if (node->right != nullptr) {
        node = node->right;
        while (node->left != nullptr) {
            node = node->left;
        }
    }
    else {
        while (node->parent != nullptr && node->parent->right == node) {
            node = node->parent;
        }
        node = node->parent;
    }
    return *this;
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::iterator::operator++(int) {
    iterator it = *this;
    ++(*this);
    return it;
}

template <class K, class V>
typename RBtree<K, V>::iterator& RBtree<K, V>::iterator::operator--() {
    if (node->left != nullptr) {
        node = node->left;
        while (node->right != nullptr) {
            node = node->right;
        }
    }
    else {
        while (node->parent != nullptr && node->parent->left == node) {
            node = node->parent;
        }
        node = node->parent;
    }
    return *this;
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::iterator::operator--(int) {
    iterator it = *this;
    --(*this);
    return it;
}

template <class K, class V>
bool RBtree<K, V>::iterator::operator==(const iterator& it) const {
    return node == it.node;
}

template <class K, class V>
bool RBtree<K, V>::iterator::operator!=(const iterator& it) const {
    return node != it.node;
}
template <class K, class V>
std::pair<K, V>& RBtree<K, V>::iterator::operator*() {
    return node->data;
}

template <class K, class V>
std::pair<K, V>* RBtree<K, V>::iterator::operator->() {
    return &(node->data);
}

template <class K, class V>
RBtree<K, V>::RBtree() : root(nullptr) {}

template <class K, class V>
RBtree<K, V>::RBtree(const RBtree& tree) {
    root = nullptr;
    if (tree.root == nullptr) {
        return;
    }
    RBtree_node<K, V>* node = tree.root;
    RBtree_node<K, V>* parent = nullptr;
    while (node != nullptr) {
        RBtree_node<K, V>* new_node = new RBtree_node<K, V>(node->data.first, node->data.second, node->color);
        new_node->parent = parent;
        if (parent == nullptr) {
            root = new_node;
        }
        else {
            if (parent->data.first > new_node->data.first) {
                parent->left = new_node;
            }
            else {
                parent->right = new_node;
            }
        }
        parent = new_node;
        node = node->left;
    }
    node = root;
    while (node != nullptr) {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr) {
                node = node->left;
            }
        }
        else {
            while (node->parent != nullptr && node->parent->right == node) {
                node = node->parent;
            }
            node = node->parent;
        }
        if (node != nullptr) {
            RBtree_node<K, V>* new_node = new RBtree_node<K, V>(node->data.first, node->data.second, node->color);
            new_node->parent = parent;
            if (parent == nullptr) {
                root = new_node;
            }
            else {
                if (parent->data.first > new_node->data.first) {
                    parent->left = new_node;
                }
                else {
                    parent->right = new_node;
                }
            }
            parent = new_node;
        }
    }
}

template <class K, class V>
RBtree<K, V>& RBtree<K, V>::operator=(const RBtree& tree) {
    if (this != &tree) {
        clear();
        root = nullptr;
        if (tree.root == nullptr) {
            return *this;
        }
        RBtree_node<K, V>* node = tree.root;
        RBtree_node<K, V>* parent = nullptr;
        while (node != nullptr) {
            RBtree_node<K, V>* new_node = new RBtree_node<K, V>(node->data.first, node->data.second, node->color);
            new_node->parent = parent;
            if (parent == nullptr) {
                root = new_node;
            }
            else {
                if (parent->data.first > new_node->data.first) {
                    parent->left = new_node;
                }
                else {
                    parent->right = new_node;
                }
            }
            parent = new_node;
            node = node->left;
        }
        node = root;
        while (node != nullptr) {
            if (node->right != nullptr) {
                node = node->right;
                while (node->left != nullptr) {
                    node = node->left;
                }
            }
            else {
                while (node->parent != nullptr && node->parent->right == node) {
                    node = node->parent;
                }
                node = node->parent;
            }
            if (node != nullptr) {
                RBtree_node<K, V>* new_node = new RBtree_node<K, V>(node->data.first, node->data.second, node->color);
                new_node->parent = parent;
                if (parent == nullptr) {
                    root = new_node;
                }
                else {
                    if (parent->data.first > new_node->data.first) {
                        parent->left = new_node;
                    }
                    else {
                        parent->right = new_node;
                    }
                }
                parent = new_node;
            }
        }
    }
    return *this;
}

template <class K, class V>
RBtree<K, V>::~RBtree() {
    clear();
}

// 用于map的增删改查
template <class K, class V>
void RBtree<K, V>::insert(const K& key, const V& value) {
    RBtree_node<K, V>* node = new RBtree_node<K, V>(key, value);
    RBtree_node<K, V>* parent = nullptr;
    RBtree_node<K, V>* temp = root;
    while (temp != nullptr) {
        parent = temp;
        if (node->data.first < temp->data.first) {
            temp = temp->left;
        }
        else if (node->data.first > temp->data.first) {
            temp = temp->right;
        }
        else {
            delete node;
            return;
        }
    }
    node->parent = parent;
    if (parent == nullptr) {
        root = node;
    }
    else {
        if (node->data.first < parent->data.first) {
            parent->left = node;
        }
        else {
            parent->right = node;
        }
    }
    fix_insert(node);
}

template <class K, class V>
void RBtree<K, V>::erase(const K& key) {
    RBtree_node<K, V>* node = root;
    while (node != nullptr) {
        if (node->data.first > key) {
            node = node->left;
        }
        else if (node->data.first < key) {
            node = node->right;
        }
        else {
            break;
        }
    }
    if (node == nullptr) {
        return;
    }
    RBtree_node<K, V>* temp = node;
    RBtree_node<K, V>* parent = node->parent;
    int color = temp->color;
    if (node->left == nullptr) {
        node = node->right;
        if (node != nullptr) {
            node->parent = parent;
        }
        if (parent == nullptr) {
            root = node;
        }
        else {
            if (parent->left == temp) {
                parent->left = node;
            }
            else {
                parent->right = node;
            }
        }
    }
    else if (node->right == nullptr) {
        node = node->left;
        if (node != nullptr) {
            node->parent = parent;
        }
        if (parent == nullptr) {
            root = node;
        }
        else {
            if (parent->left == temp) {
                parent->left = node;
            }
            else {
                parent->right = node;
            }
        }
    }
    else {
        temp = node->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        color = temp->color;
        node->data.first = temp->data.first;
        node->data.second = temp->data.second;
        parent = temp->parent;
        node = temp->right;
        if (parent->left == temp) {
            parent->left = node;
        }
        else {
            parent->right = node;
        }
        if (node != nullptr) {
            node->parent = parent;
        }
    }
    if (color == BLACK) {
        fix_erase(node, parent);
    }
    delete temp;
}

template <class K, class V>
void RBtree<K, V>::fix_insert(RBtree_node<K, V>* node) {
    while (node->parent != nullptr && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            RBtree_node<K, V>* uncle = node->parent->parent->right;
            if (uncle != nullptr && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                right_rotate(node->parent->parent);
            }
        }
        else {
            RBtree_node<K, V>* uncle = node->parent->parent->left;
            if (uncle != nullptr && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                left_rotate(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

template <class K, class V>
void RBtree<K, V>::fix_erase(RBtree_node<K, V>* node, RBtree_node<K, V>* parent) {
    while (node != root && (node == nullptr || node->color == BLACK)) {
        if (node == parent->left) {
            RBtree_node<K, V>* sibling = parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                left_rotate(parent);
                sibling = parent->right;
            }
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK)) {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else {
                if (sibling->right == nullptr || sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    right_rotate(sibling);
                    sibling = parent->right;
                }
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                left_rotate(parent);
                node = root;
                break;
            }
        }
        else {
            RBtree_node<K, V>* sibling = parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                right_rotate(parent);
                sibling = parent->left;
            }
            if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                (sibling->left == nullptr || sibling->left->color == BLACK)) {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else {
                if (sibling->left == nullptr || sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    left_rotate(sibling);
                    sibling = parent->left;
                }
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                right_rotate(parent);
                node = root;
                break;
            }
        }
    }
    if (node != nullptr) {
        node->color = BLACK;
    }
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::find(const K& key) const {
    RBtree_node<K, V>* node = root;
    while (node != nullptr) {
        if (node->data.first > key) {
            node = node->left;
        }
        else if (node->data.first < key) {
            node = node->right;
        }
        else {
            return iterator(node);
        }
    }
    return end();
}

template <class K, class V>
V& RBtree<K, V>::operator[](const K& key) {
    RBtree_node<K, V>* node = root;
    while (node != nullptr) {
        if (node->data.first > key) {
            node = node->left;
        }
        else if (node->data.first < key) {
            node = node->right;
        }
        else {
            return node->data.second;
        }
    }
    insert(key, V());
    return operator[](key);
}

// 用于multimap的增删查
template <class K, class V>
void RBtree<K, V>::multi_insert(const K& key, const V& value) {
    RBtree_node<K, V>* node = new RBtree_node<K, V>(key, value);
    RBtree_node<K, V>* parent = nullptr;
    RBtree_node<K, V>* temp = root;
    while (temp != nullptr) {
        parent = temp;
        if (node->data.first < temp->data.first) {
            temp = temp->left;
        }
        else {
            temp = temp->right;
        }
    }
    node->parent = parent;
    if (parent == nullptr) {
        root = node;
    }
    else {
        if (node->data.first < parent->data.first) {
            parent->left = node;
        }
        else {
            parent->right = node;
        }
    }
    fix_insert(node);
}

template <class K, class V>
void RBtree<K, V>::multi_erase(const K& key) {
    RBtree_node<K, V>* current = find(key).node;

    while (current != nullptr) {
        RBtree_node<K, V>* nodeToDelete = current;

        if (nodeToDelete->data.first == key) {
            RBtree_node<K, V>* nodeToReplace = nullptr;
            if (nodeToDelete->left == nullptr || nodeToDelete->right == nullptr) {
                nodeToReplace = nodeToDelete;
            }
            else {
                nodeToReplace = next_node(nodeToDelete);
            }

            RBtree_node<K, V>* childOfNodeToReplace = nullptr;
            if (nodeToReplace->left != nullptr) {
                childOfNodeToReplace = nodeToReplace->left;
            }
            else {
                childOfNodeToReplace = nodeToReplace->right;
            }

            if (childOfNodeToReplace != nullptr) {
                childOfNodeToReplace->parent = nodeToReplace->parent;
            }

            if (nodeToReplace->parent == nullptr) {
                root = childOfNodeToReplace;
            }
            else if (nodeToReplace == nodeToReplace->parent->left) {
                nodeToReplace->parent->left = childOfNodeToReplace;
            }
            else {
                nodeToReplace->parent->right = childOfNodeToReplace;
            }

            if (nodeToReplace != nodeToDelete) {
                nodeToDelete->data.first = nodeToReplace->data.first;
                nodeToDelete->data.second = nodeToReplace->data.second;
            }

            if (nodeToReplace->color == BLACK) {
                fix_erase(childOfNodeToReplace, nodeToReplace->parent);
            }

            delete nodeToReplace;
            current = find(key).node;
        }
    }
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::multi_find(const K& key) {
    RBtree_node<K, V>* node = root;
    while (node != nullptr) {
        if (node->data.first > key) {
            node = node->left;
        }
        else if (node->data.first < key) {
            node = node->right;
        }
        else {
            return iterator(node);
        }
    }
    return end();
}

template <class K, class V>
RBtree_node<K, V>* RBtree<K, V>::next_node(RBtree_node<K, V>* node) const {
    if (node->right != nullptr) {
        node = node->right;
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    else {
        RBtree_node<K, V>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }
}


template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::begin() const {
    RBtree_node<K, V>* node = root;
    if (node != nullptr) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return iterator(node);
    }
    return end();
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::end() const {
    return iterator(nullptr);
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::lower_bound(const K& key) {
    iterator it = begin();
    while (it != end() && it.node->data.first < key) {
        ++it;
    }
    return it;
}

template <class K, class V>
typename RBtree<K, V>::iterator RBtree<K, V>::upper_bound(const K& key) {
    iterator it = begin();
    while (it != end() && it.node->data.first <= key) {
        ++it;
    }
    return it;
}

template <class K, class V>
std::pair<typename RBtree<K, V>::iterator, typename RBtree<K, V>::iterator> RBtree<K, V>::multi_equal_range(const K& key) {
    iterator first = lower_bound(key);
    iterator last = upper_bound(key);
    return std::make_pair(first, last);
}

template <class K, class V>
bool RBtree<K, V>::empty() {
    return root == nullptr;
}

template <class K, class V>
size_t RBtree<K, V>::size() {
    size_t count = 0;
    for (iterator it = begin(); it != end(); ++it) {
        ++count;
    }
    return count;
}

template <class K, class V>
void RBtree<K, V>::clear() {
    clear_recursive(root);
    root = nullptr;
}

template <class K, class V>
void RBtree<K, V>::clear_recursive(RBtree_node<K, V>* node) {
    if (node == nullptr) {
        return;
    }
    clear_recursive(node->left);
    clear_recursive(node->right);
    delete node;
}

template <class K, class V>
void RBtree<K, V>::left_rotate(RBtree_node<K, V>* node) {
    RBtree_node<K, V>* temp = node->right;
    node->right = temp->left;
    if (temp->left != nullptr) {
        temp->left->parent = node;
    }
    temp->parent = node->parent;
    if (node->parent == nullptr) {
        root = temp;
    }
    else if (node == node->parent->left) {
        node->parent->left = temp;
    }
    else {
        node->parent->right = temp;
    }
    temp->left = node;
    node->parent = temp;
}

template <class K, class V>
void RBtree<K, V>::right_rotate(RBtree_node<K, V>* node) {
    RBtree_node<K, V>* temp = node->left;
    node->left = temp->right;
    if (temp->right != nullptr) {
        temp->right->parent = node;
    }
    temp->parent = node->parent;
    if (node->parent == nullptr) {
        root = temp;
    }
    else if (node == node->parent->left) {
        node->parent->left = temp;
    }
    else {
        node->parent->right = temp;
    }
    temp->right = node;
    node->parent = temp;
}

template <class K, class V>
void RBtree<K, V>::print() {
    for (iterator it = begin(); it != end(); ++it) {
        std::cout << it.node->data.first << ":" << it.node->data.second << " ";
    }
    std::cout << std::endl;
}

// template <class K, class V>
// RBtree_node<K, V>* RBtree<K, V>::minimum(RBtree_node<K, V>* node) const {
//     while (node->left != nullptr) {
//         node = node->left;
//     }
//     return node;
// }

// template <class K, class V>
// RBtree_node<K, V>* RBtree<K, V>::maximum(RBtree_node<K, V>* node) const {
//     while (node->right != nullptr) {
//         node = node->right;
//     }
//     return node;
// }

// template <class K, class V>
// RBtree_node<K, V>* RBtree<K, V>::predecessor(RBtree_node<K, V>* node) const {
//     if (node->left != nullptr) {
//         return maximum(node->left);
//     }
//     RBtree_node<K, V>* parent = node->parent;
//     while (parent != nullptr && node == parent->left) {
//         node = parent;
//         parent = parent->parent;
//     }
//     return parent;
// }

// template <class K, class V>
// RBtree_node<K, V>* RBtree<K, V>::successor(RBtree_node<K, V>* node) const {
//     if (node->right != nullptr) {
//         return minimum(node->right);
//     }
//     RBtree_node<K, V>* parent = node->parent;
//     while (parent != nullptr && node == parent->right) {
//         node = parent;
//         parent = parent->parent;
//     }
//     return parent;
// }

// template <class K, class V>
// bool RBtree<K, V>::is_red(RBtree_node<K, V>* node) const {
//     return node != nullptr && node->color == RED;
// }

}

#endif // CORE_MTL_RBTREE_H_