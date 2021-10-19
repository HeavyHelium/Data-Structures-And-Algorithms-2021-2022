#ifndef DARRAY_H
#define DARRAY_H

#include <cstddef>
#include <algorithm>

template<class T>
class dynamicArray
{
    static constexpr std::size_t INITIAL_CAPACITY = 6;

    std::size_t m_capacity;
    std::size_t m_size;
    T* array;
public:
    class iterator
    {
        friend class dynamicArray;
        T* ptr;
        iterator(T* ptr)
            : ptr(ptr)
        {}
    public:

        T& operator*()
        { return *ptr; }
        const T& operator*() const
        { return *ptr; }
        T& operator->()
        { return ptr; }
        const T& operator->() const
        { return ptr; }
        iterator& operator++()
        { ++ptr; return (*this); }
        iterator& operator++(int)
        {
            iterator copy = *this;
            return ++(*this);
        }
        iterator& operator--()
        { --ptr; return (*this); }
        iterator& operator--(int)
        {
            iterator copy = *this;
            return --(*this);
        }
        iterator operator+(std::size_t offset)
        { return ptr + offset; }
        iterator operator-(std::size_t offset)
        { return ptr - offset; }
        bool operator!=(const iterator& other)
        { return ptr != other.ptr; }

    };
    /*
    class elementProxy
    {
        std::size_t index;
        T* arrayPtr;
    public:
        elementProxy(std::size_t index, T* array)
            : index(index), arrayPtr(array)
        {}
        elementProxy& operator=(const elementProxy& other)
        { index = other.index; return *this; }
        operator T() const
        { return arrayPtr[index]; }


    };*/

    dynamicArray(std::size_t size);
    dynamicArray();
    dynamicArray(const dynamicArray& other);
    dynamicArray(dynamicArray&& other);
    dynamicArray& operator=(const dynamicArray& other);
    dynamicArray& operator=(dynamicArray&& other);
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;
    void clear();
    void reserve(std::size_t newCapacity);
    void resize(std::size_t size);
    void resize(std::size_t size, const T& value);
    void push_back(const T& element);
    void push_back(T&& element);
    void swap(dynamicArray& other);
    void pop_back();
    iterator begin();
    iterator end();
    const iterator begin() const;
    const iterator end() const;
    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    ~dynamicArray();
private:
    std::size_t calculateGrowth(std::size_t newSize) const;
};


template<class T>

dynamicArray<T>::dynamicArray(std::size_t size)
    : m_size(0), array(new T[m_capacity]), m_capacity(size)
{}

template<class T>
dynamicArray<T>::dynamicArray()
    : m_capacity(INITIAL_CAPACITY), m_size(0), array(new T[m_capacity])
{}

template<class T>
dynamicArray<T>::dynamicArray(const dynamicArray& other)
    : m_size(other.m_size), array(new T[m_capacity]), m_capacity(other.m_capacity)
{
    try
    {
        for(std::size_t i = 0; i < m_size; ++i)
            array[i] = other.array[i];
    }
    catch(...)
    {
        delete array;
        throw;
    }
}

template<class T>
dynamicArray<T>::dynamicArray(dynamicArray&& other)
    :m_size(other.m_size), array(other.array), m_capacity(other.m_capacity)
{
    other.array = nullptr;
    other.m_size = 0;
    other.capacity = 0;
}

template<class T>
dynamicArray<T>& dynamicArray<T>::operator=(const dynamicArray& other)
{
    if(this != &other)
    {
        T* temp(new T[other.capacity]);
        try
        {
            for(std::size_t i = 0; i < other.m_size; ++i)
                temp[i] = other.array[i];
        }
        catch(...)
        {
            delete temp;
            throw;
        }
        array = temp;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }
    return *this;
}

template<class T>
dynamicArray<T>& dynamicArray<T>::operator=(dynamicArray&& other)
{
    std::swap(array, other.array);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

template<class T>
T& dynamicArray<T>::operator[](std::size_t index)
{ return array[index]; }


template<class T>
const T& dynamicArray<T>::operator[](std::size_t index) const
{ return array[index]; }

template<class T>
void dynamicArray<T>::clear()
{
    delete array;
    m_size = m_capacity = 0;
}

template<class T>
void dynamicArray<T>::reserve(std::size_t newCapacity)
{
    if(m_capacity > newCapacity) return;
    std::size_t temp = calculateGrowth(m_size);
    T* arr = new T[temp];
    try
    {
        for(std::size_t i = 0; i < m_size; ++i)
            arr[i] = array[i];
    }
    catch(...) { delete array; throw; }
    m_capacity = temp;
    array = arr;

}

template<class T>
void dynamicArray<T>::resize(std::size_t newSize)
{
    reserve(newSize);
    m_size = newSize;
}

template<class T>
void dynamicArray<T>::resize(std::size_t newSize, const T& value)
{
    if(m_capacity > newSize) { m_size = newSize; return; }
    std::size_t temp = calculateGrowth(newSize);
    T* arr = new T[temp];
    try
    {
        for(std::size_t i = 0; i < m_size; ++i)
            arr[i] = array[i];
        for(std::size_t i = m_size - 1; i < newSize; ++i)
            arr[i] = value;
    }
    catch(...) { delete array; throw; }
    m_capacity = temp;
    array = arr;
    m_size = newSize;
}

template<class T>
void dynamicArray<T>::push_back(const T& element)
{
    if(m_size == m_capacity)
        reserve(calculateGrowth(m_size + 1));
    array[m_size++] = element;
}

template<class T>
void dynamicArray<T>::push_back(T&& element)
{
    if(m_size == m_capacity)
        reserve(calculateGrowth(m_size + 1));
    array[m_size++] = element;
}

template<class T>
void dynamicArray<T>::swap(dynamicArray& other)
{
    std::swap(array, other.array);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

template<class T>
void dynamicArray<T>::pop_back()
{
    assert("ensure array is non-empty in order to pop elements" && !empty());
    --m_size;
}

template<class T>
typename dynamicArray<T>::iterator dynamicArray<T>::begin()
{ return iterator(array); }

template<class T>
typename dynamicArray<T>::iterator dynamicArray<T>::end()
{ return iterator(array + size()); }

template<class T>
const typename dynamicArray<T>::iterator dynamicArray<T>::begin() const
{ return iterator(array); }

template<class T>
const typename dynamicArray<T>::iterator dynamicArray<T>::end() const
{ return iterator(array + size()); }

template<class T>
std::size_t dynamicArray<T>::size() const
{ return m_size; }

template<class T>
bool dynamicArray<T>::empty() const
{ return m_size == 0; }

template<class T>
std::size_t dynamicArray<T>::capacity() const
{ return m_capacity; }

template<class T>
dynamicArray<T>::~dynamicArray()
{ clear(); }

template<class T>
std::size_t dynamicArray<T>::calculateGrowth(std::size_t newCapacity) const
{
    if(newCapacity < m_capacity)
        return m_capacity;
    std::size_t temp = 2 * m_capacity;
    return (temp > newCapacity) ? temp : newCapacity;
}


#endif
