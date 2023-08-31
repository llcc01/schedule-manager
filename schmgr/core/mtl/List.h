#pragma once

#ifndef CORE_MTL_LIST_H_
#define CORE_MTL_LIST_H_

#include <initializer_list>

namespace core::mtl
{

template <class T>
struct ListNode {
    T val;
    ListNode* prev;
    ListNode* next;
    ListNode(T val) : val(val), prev(nullptr), next(nullptr) {}
};

template <class T>
class List {
public:
    class iterator;

private:
    ListNode<T>* _head;     // 指向链表的第一个元素
    ListNode<T>* _tail;     // 指向链表的最后一个元素
    size_t _size;           // 链表中元素的个数

public:
    // 函数声明
    List() : _head(nullptr), _tail(nullptr), _size(0) {}    // 构造函数
    List(size_t n, T val);                                  // 构造函数
    List(const List<T>& other);                             // 拷贝构造函数
    List(List<T>&& other);                                  // 移动构造函数
    List(std::initializer_list<T> l);                       // 初始化列表构造函数
    ~List();                                                // 析构函数

    List<T>& operator=(const List<T>& other);               // 拷贝赋值运算符
    List<T>& operator=(List<T>&& other);                    // 移动赋值运算符

    void push_back(T val);                                  // 在链表尾部插入一个元素
    void push_front(T val);                                 // 在链表头部插入一个元素
    void pop_back();                                        // 删除链表尾部的一个元素
    void pop_front();                                       // 删除链表头部的一个元素

    T& front();                                             // 返回链表头部元素的引用
    T& back();                                              // 返回链表尾部元素的引用

    iterator begin() const;                                       // 返回指向链表第一个元素的迭代器
    iterator end() const;                                         // 返回指向链表最后一个元素的迭代器

    size_t size() const;                                    // 返回链表中元素的个数
    bool empty() const;                                     // 判断链表是否为空
    void clear();                                           // 清空链表

    void insert(iterator pos, size_t n, const T& value);    // 在pos位置插入n个值为value的元素
    void insert(iterator pos, const T& value)    // 在pos位置插入1个值为value的元素
    {
        insert(pos, 1, value);
    }
    void erase(iterator pos, iterator last);                // 删除[pos, last)中的元素
    void erase(iterator pos)                                // 删除pos位置的元素
    {
        erase(pos, pos + 1);
    }
    void remove(const T& val);                              // 删除链表中所有值为val的元素
    void reverse();                                         // 反转链表
    void sort();                                            // 对链表进行排序(归并, 升序)

    void swap(List<T>& other);                              // 交换两个链表
    void resize(size_t new_size);                           // 改变链表的大小
    void resize(size_t new_size, const T& val);             // 改变链表的大小，并用val填充新的位置

    T& operator[](size_t index);                            // 下标运算符重载

    // void print() const;                                     // 打印链表(测试用)
};

template <class T>
class List<T>::iterator {
public:
    ListNode<T>* _node;     // 指向链表中的一个节点

public:
    // 函数声明
    iterator() : _node(nullptr) {}                          // 构造函数
    iterator(ListNode<T>* node) : _node(node) {}            // 构造函数
    iterator(const iterator& other) : _node(other._node) {} // 拷贝构造函数
    ~iterator() {}                                          // 析构函数

    iterator& operator=(const iterator& other);             // 拷贝赋值运算符

    iterator operator+(int n) const;                     // 加法运算符重载
    iterator operator-(int n) const;                     // 减法运算符重载
    iterator& operator++();                                 // 前置++运算符重载
    iterator operator++(int);                               // 后置++运算符重载
    iterator& operator--();                                 // 前置--运算符重载
    iterator operator--(int);                               // 后置--运算符重载

    bool operator==(const iterator& other) const;           // 相等运算符重载
    bool operator!=(const iterator& other) const;           // 不等运算符重载

    T& operator*();                                         // 解引用运算符重载
    T* operator->();                                        // 指针运算符重载
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};


template <class T>
typename List<T>::iterator List<T>::iterator::operator+(int n) const {
    iterator temp = *this;
    for (size_t i = 0; i < n; ++i) {
        temp._node = temp._node->next;
    }
    return temp;
}

template <class T>
typename List<T>::iterator List<T>::iterator::operator-(int n) const {
    iterator temp = *this;
    for (size_t i = 0; i < n; ++i) {
        temp._node = temp._node->prev;
    }
    return temp;
}

template <class T>
typename List<T>::iterator& List<T>::iterator::operator=(const iterator& other) {
    if (this != &other) {
        _node = other._node;
    }
    return *this;
}

template <class T>
typename List<T>::iterator& List<T>::iterator::operator++() {
    _node = _node->next;
    return *this;
}

template <class T>
typename List<T>::iterator List<T>::iterator::operator++(int) {
    iterator temp = *this;
    _node = _node->next;
    return temp;
}

template <class T>
typename List<T>::iterator& List<T>::iterator::operator--() {
    _node = _node->prev;
    return *this;
}

template <class T>
typename List<T>::iterator List<T>::iterator::operator--(int) {
    iterator temp = *this;
    _node = _node->prev;
    return temp;
}

template <class T>
bool List<T>::iterator::operator==(const iterator& other) const {
    return _node == other._node;
}

template <class T>
bool List<T>::iterator::operator!=(const iterator& other) const {
    return _node != other._node;
}

template <class T>
T& List<T>::iterator::operator*() {
    return _node->val;
}

template <class T>
T* List<T>::iterator::operator->() {
    return &_node->val;
}

template <class T>
List<T>::List(size_t n, T val) {
    _head = new ListNode<T>(val);
    _tail = _head;
    _size = 1;
    for (size_t i = 1; i < n; ++i) {
        push_back(val);
    }
}

template <class T>
List<T>::List(const List<T>& other) {
    _head = new ListNode<T>(other._head->val);
    _tail = _head;
    _size = 1;
    for (iterator p = other._head->next; p != nullptr; p = p->next) {
        push_back(p->val);
    }
}

template <class T>
List<T>::List(List<T>&& other) {
    _head = other._head;
    _tail = other._tail;
    _size = other._size;
    other._head = nullptr;
    other._tail = nullptr;
    other._size = 0;
}

template <class T>
List<T>::List(std::initializer_list<T> l) {
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        push_back(*it);
    }
}

template <class T>
List<T>::~List() {
    clear();
}

template <class T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this != &other) {
        clear();
        _head = new ListNode<T>(other._head->val);
        _tail = _head;
        _size = 1;
        for (ListNode<T>* p = other._head->next; p != nullptr; p = p->next) {
            push_back(p->val);
        }
    }
    return *this;
}

template <class T>
List<T>& List<T>::operator=(List<T>&& other) {
    if (this != &other) {
        clear();
        _head = other._head;
        _tail = other._tail;
        _size = other._size;
        other._head = nullptr;
        other._tail = nullptr;
        other._size = 0;
    }
    return *this;
}

template <class T>
void List<T>::push_back(T val) {
    if (_head == nullptr) {
        _head = new ListNode<T>(val);
        _tail = _head;
    }
    else {
        _tail->next = new ListNode<T>(val);
        _tail->next->prev = _tail;
        _tail = _tail->next;
    }
    ++_size;
}

template <class T>
void List<T>::push_front(T val) {
    if (_head == nullptr) {
        _head = new ListNode<T>(val);
        _tail = _head;
    }
    else {
        _head->prev = new ListNode<T>(val);
        _head->prev->next = _head;
        _head = _head->prev;
    }
    ++_size;
}

template <class T>
void List<T>::pop_back() {
    if (_head == nullptr) {
        return;
    }
    else if (_head == _tail) {
        delete _head;
        _head = nullptr;
        _tail = nullptr;
    }
    else {
        iterator p = _tail;
        _tail = _tail->prev;
        _tail->next = nullptr;
        delete p;
    }
    --_size;
}

template <class T>
void List<T>::pop_front() {
    if (_head == nullptr) {
        return;
    }
    else if (_head == _tail) {
        delete _head;
        _head = nullptr;
        _tail = nullptr;
    }
    else {
        ListNode<T>* p = _head;
        _head = _head->next;
        _head->prev = nullptr;
        delete p;
    }
    --_size;
}

template <class T>
T& List<T>::front() {
    return _head->val;
}

template <class T>
T& List<T>::back() {
    return _tail->val;
}

template <class T>
typename List<T>::iterator List<T>::begin() const {
    return iterator(_head);
}

template <class T>
typename List<T>::iterator List<T>::end() const {
    return iterator(nullptr);
}

template <class T>
size_t List<T>::size() const {
    return _size;
}

template <class T>
bool List<T>::empty() const {
    return _size == 0;
}

template <class T>
void List<T>::clear() {
    while (_head != nullptr) {
        ListNode<T>* p = _head;
        _head = _head->next;
        delete p;
    }
    _tail = nullptr;
    _size = 0;
}

template <class T>
void List<T>::insert(iterator pos, size_t n, const T& val) {
    if (pos == end()) {
        for (size_t i = 0; i < n; ++i) {
            push_back(val);
        }
    }
    else {
        ListNode<T>* p = pos._node;
        for (size_t i = 0; i < n; ++i) {
            ListNode<T>* q = new ListNode<T>(val);
            q->next = p;
            q->prev = p->prev;
            if (p->prev != nullptr) {
                p->prev->next = q;
            }
            else {
                _head = q;
            }
            p->prev = q;
            ++_size;
        }
    }
};

template <class T>
void List<T>::erase(iterator pos, iterator last) {
    if (pos == end()) {
        return;
    }
    else if (pos == begin() && last == end()) {
        clear();
    }
    else {
        ListNode<T>* p = pos._node;
        ListNode<T>* q = last._node;
        if (p->prev != nullptr) {
            p->prev->next = q;
        }
        else {
            _head = q;
        }
        q->prev = p->prev;
        while (p != q) {
            ListNode<T>* r = p;
            p = p->next;
            delete r;
            --_size;
        }
    }
}

template <class T>
void List<T>::swap(List<T>& other) {
    std::swap(_head, other._head);
    std::swap(_tail, other._tail);
    std::swap(_size, other._size);
}

template <class T>
void List<T>::resize(size_t new_size) {
    if (new_size < _size) {
        for (size_t i = 0; i < _size - new_size; ++i) {
            pop_back();
        }
    }
    else if (new_size > _size) {
        for (size_t i = 0; i < new_size - _size; ++i) {
            push_back(T());
        }
    }
}

template <class T>
void List<T>::resize(size_t new_size, const T& val) {
    if (new_size < _size) {
        for (size_t i = 0; i < _size - new_size; ++i) {
            pop_back();
        }
    }
    else if (new_size > _size) {
        for (size_t i = 0; i < new_size - _size; ++i) {
            push_back(val);
        }
    }
}

template <class T>
void List<T>::remove(const T& val) {
    ListNode<T>* p = _head;
    while (p != nullptr) {
        if (p->val == val) {
            if (p->prev != nullptr) {
                p->prev->next = p->next;
            }
            else {
                _head = p->next;
            }
            if (p->next != nullptr) {
                p->next->prev = p->prev;
            }
            else {
                _tail = p->prev;
            }
            ListNode<T>* q = p;
            p = p->next;
            delete q;
            --_size;
        }
        else {
            p = p->next;
        }
    }
}

template <class T>
void List<T>::reverse() {
    ListNode<T>* p = _head;
    while (p != nullptr) {
        std::swap(p->prev, p->next);
        p = p->prev;
    }
    std::swap(_head, _tail);
}

template <class T>
void List<T>::sort() {
    if (_size <= 1) {
        return;
    }
    List<T> left, right;
    ListNode<T>* p = _head;
    for (size_t i = 0; i < _size / 2; ++i) {
        left.push_back(p->val);
        p = p->next;
    }
    for (size_t i = _size / 2; i < _size; ++i) {
        right.push_back(p->val);
        p = p->next;
    }
    left.sort();
    right.sort();
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
    while (!left.empty() && !right.empty()) {
        if (left.front() < right.front()) {
            push_back(left.front());
            left.pop_front();
        }
        else {
            push_back(right.front());
            right.pop_front();
        }
    }
    while (!left.empty()) {
        push_back(left.front());
        left.pop_front();
    }
    while (!right.empty()) {
        push_back(right.front());
        right.pop_front();
    }
}

template <class T>
T& List<T>::operator[](size_t index) {
    ListNode<T>* p = _head;
    for (size_t i = 0; i < index; ++i) {
        p = p->next;
    }
    return p->val;
}

// template <class T>
// void List<T>::print() const {
//     ListNode<T>* p = _head;
//     std::cout << "List:";
//     while (p != nullptr) {
//         std::cout << p->val << " ";
//         p = p->next;
//     }
//     std::cout << std::endl;
// }

}

#endif // CORE_MTL_LIST_H_