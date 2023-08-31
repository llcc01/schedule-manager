#pragma once

#include <initializer_list>

#ifndef CORE_MTL_VECTOR_H_
#define CORE_MTL_VECTOR_H_

namespace core::mtl
{

template <class T>

class Vector {
public:
    using iterator = T*;        // 迭代器

private:
    iterator _start;            // 指向数组的第一个元素
    iterator _finish;           // 指向数组的最后一个元素的下一个位置
    iterator _end_of_storage;   // 指向数组的容量大小的位置

public:
    // 函数声明
    Vector();                                   // 默认构造函数
    Vector(size_t n, const T& value = T());     // 构造函数, 初始化n个value
    Vector(Vector<T>&&);                        // 移动构造函数
    Vector(const Vector<T>&);                   // 拷贝构造函数
    Vector(std::initializer_list<T> l);         // 初始化列表构造函数
    ~Vector();                                  // 析构函数

    Vector<T>& operator=(Vector<T>&&);          // 移动赋值运算符, 和移动构造函数同理
    Vector<T>& operator=(const Vector<T>&);     // 拷贝赋值运算符, 和拷贝构造函数同理

    T& operator[](size_t pos);                  // 下标运算符
    const T& operator[](size_t pos) const;      // 下标运算符, 常量对象

public:
    iterator begin() const;                           // 返回指向数组第一个元素的迭代器
    iterator end() const;                             // 返回指向数组最后一个元素的下一个位置的迭代器

    T& front() const;                                 // 返回数组第一个元素的引用
    T& back() const;                                  // 返回数组最后一个元素的引用

    size_t size() const;                        // 返回数组中元素的个数
    size_t capacity() const;                    // 返回数组的容量
    bool empty() const;                         // 判断数组是否为空

    void push_back(const T& value);                         // 在数组末尾添加一个元素
    void pop_back();                                        // 删除数组末尾的一个元素
    void erase(iterator first, iterator last);              // 删除[first, last)区间的元素
    void erase(iterator pos)                                // 删除pos位置的元素
    {
        erase(pos, pos + 1);
    }
    void insert(iterator pos, size_t n, const T& value);    // 在pos位置的前面插入n个value

    void clear();                                           // 清空数组
    void resize(size_t n, const T& value = T());            // 重新设置数组的大小
    void reserve(size_t n);                                 // 重新设置数组的容量
    void sort(iterator first, iterator last);               // 对[first, last)区间的元素进行排序(快排, 升序)

    void swap(Vector<T>& other);                            // 交换两个数组

    // void print();                                           // 打印数组(测试用)
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = size_t;
};

template <class T>
Vector<T>::Vector() { // 默认初始容量是16
    _start = new T[16];
    _finish = _start;
    _end_of_storage = _start + 16;
}

template <class T>
Vector<T>::Vector(size_t n, const T& value) {
    if (n < 16) {
        _start = new T[16];
        _finish = _start + n;
        _end_of_storage = _start + 16;
    }
    else {
        _start = new T[n];
        _finish = _start + n;
        _end_of_storage = _finish;
    }
    for (size_t i = 0; i < n; ++i)
        _start[i] = value;
}

template <class T>
Vector<T>::Vector(Vector<T>&& other) {
    _start = other._start;
    _finish = other._finish;
    _end_of_storage = other._end_of_storage;
    other._start = nullptr;
    other._finish = nullptr;
    other._end_of_storage = nullptr;
}

template <class T>
Vector<T>::Vector(const Vector<T>& other) {
    _start = new T[other.size()];
    _finish = _start + other.size();
    _end_of_storage = _finish;
    for (size_t i = 0; i < other.size(); ++i)
        _start[i] = other._start[i];
}

template <class T>
Vector<T>::Vector(std::initializer_list<T> l) {
    if (l.size() < 16) {
        _start = new T[16];
        _finish = _start + l.size();
        _end_of_storage = _start + 16;
    }
    else {
        _start = new T[l.size()];
        _finish = _start + l.size();
        _end_of_storage = _finish;
    }
    size_t i = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        _start[i] = *it;
        ++i;
    }
}

template <class T>
Vector<T>::~Vector() {
    delete[] _start;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) {
    if (this == &other)
        return *this;
    delete[] _start;
    _start = other._start;
    _finish = other._finish;
    _end_of_storage = other._end_of_storage;
    other._start = nullptr;
    other._finish = nullptr;
    other._end_of_storage = nullptr;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this == &other)
        return *this;
    delete[] _start;
    _start = new T[other.size()];
    _finish = _start + other.size();
    _end_of_storage = _finish;
    for (size_t i = 0; i < other.size(); ++i)
        _start[i] = other._start[i];
    return *this;
}

template <class T>
T& Vector<T>::operator[](size_t pos) {
    return _start[pos];
}

template <class T>
const T& Vector<T>::operator[](size_t pos) const {
    return _start[pos];
}

template <class T>
typename Vector<T>::iterator Vector<T>::begin() const {
    return _start;
}

template <class T>
typename Vector<T>::iterator Vector<T>::end() const {
    return _finish;
}

template <class T>
size_t Vector<T>::size() const {
    return _finish - _start;
}

template <class T>
size_t Vector<T>::capacity() const {
    return _end_of_storage - _start;
}

template <class T>
bool Vector<T>::empty() const {
    return _start == _finish;
}

template <class T>
void Vector<T>::push_back(const T& value) {

    if (_finish == _end_of_storage) {
        size_t new_capacity = capacity() == 0 ? 16 : capacity() * 2;
        reserve(new_capacity);
    }
    *_finish = value;
    ++_finish;
}

template <class T>
void Vector<T>::pop_back() {
    --_finish;
}

template <class T>
void Vector<T>::erase(iterator first, iterator last) { // 删除[first, last)区间的元素
    if (first == last)
        return;
    iterator it = first;
    while (last != _finish) {
        *it = *last;
        ++it;
        ++last;
    }
    _finish = it;
}

template <class T>
void Vector<T>::insert(iterator pos, size_t n, const T& value) {
    if (n == 0)
        return;
    if (size() + n > capacity()) {
        size_t new_capacity = capacity() == 0 ? 16 : capacity() * 2;
        while (new_capacity < size() + n)
            new_capacity *= 2;
        reserve(new_capacity);
    }
    iterator it = _finish - 1;
    while (it >= pos) {
        *(it + n) = *it;
        --it;
    }
    for (size_t i = 0; i < n; ++i)
        *(pos + i) = value;
    _finish += n;
}

template <class T>
void Vector<T>::clear() {
    _finish = _start;
}

template <class T>
void Vector<T>::resize(size_t n, const T& value) {
    if (n < size()) {
        _finish = _start + n;
    }
    else {
        if (n > capacity()) {
            reserve(n);
        }
        for (size_t i = size(); i < n; ++i)
            _start[i] = value;
        _finish = _start + n;
    }
}

template <class T>
void Vector<T>::reserve(size_t n) {
    size_t old_size = size();
    if (n > capacity()) {
        T* new_start = new T[n];
        for (size_t i = 0; i < old_size; ++i)
            new_start[i] = _start[i];
        delete[] _start;
        _start = new_start;
        _finish = _start + old_size;
        _end_of_storage = _start + n;
    }
}

template <class T>
void Vector<T>::sort(iterator first, iterator last) {
    if (first == last)
        return;
    iterator it = first;
    iterator it2 = first + 1;
    while (it2 != last) {
        if (*it2 < *first) {
            ++it;
            std::swap(*it, *it2);
        }
        ++it2;
    }
    std::swap(*first, *it);
    sort(first, it);
    sort(it + 1, last);
}

template <class T>
void Vector<T>::swap(Vector<T>& other) {
    std::swap(_start, other._start);
    std::swap(_finish, other._finish);
    std::swap(_end_of_storage, other._end_of_storage);
}

template <class T>
T& Vector<T>::front() const {
    return *_start;
}

template <class T>
T& Vector<T>::back() const {
    return *(_finish - 1);
}

// template <class T>
// void Vector<T>::print() {
//     std::cout << "Vector:";
//     for (size_t i = 0; i < size(); ++i)
//         std::cout << _start[i] << " ";
//     std::cout << std::endl;
// }

} // namespace core::mtl

#endif // CORE_MTL_VECTOR_H_
