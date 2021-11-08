#pragma once

#include <cassert>
#include <stdexcept>
#include <cstddef>

template<class T>
class box
{
public:
    T value = 0;
    box* next = 0;
    box() = default;
    box(const T& value, box* next = nullptr)
        : value(value), next(next)
    {}
    bool has_next() const
    { return next; }
};
template<class T>
class box_allocator
{
public:
    box<T>* allocate()
    {
        return new box<T>();
    }

    box<T>* allocate(const T& value, box<T>* next = nullptr)
    {
        return new box<T>(value, next);
    }

    void free(box<T>* p)
    {
        delete p;
    }
};
template<class T>
class debug_allocator
{
    std::size_t allocations_count = 0;
    public:
    box<T>* allocate()
    {
        box<T>* result = new box<T>();
        ++allocations_count;
        return result;
    }

    void free(box<T>* p)
    {
        delete p;
        --allocations_count;
    }
    std::size_t currently_allocated() const
    { return allocations_count; }

};

template<class T, class AllocatorType>
class linked_stack
{

/// data members
    box<T>* first = nullptr;
    std::size_t boxes_count = 0;
    static inline AllocatorType alloc;
private:
    static box<T>* clone_chain(box<T>* p)
    {
        if(!p) return nullptr;
        box<T>* result = nullptr;
        try
        {
            result = alloc.allocate(p -> value);
            box<T>* current = result;

            while(p = p -> next)
            {
                current -> next = alloc.allocate(p -> value);
                current = current -> next;
            }
        }
        catch(const std::bad_alloc&)
        {
            free_chain(result);
        }
        return result;
    }

    static void free_chain(box<T>* p)
    {
        while(p)
        {
            box<T>* temp = p;
            p = p -> next;
            delete temp;
        }
    }


public:
    linked_stack() = default;

    linked_stack(const linked_stack& other)
    {
        first = clone_chain(other.first);
        boxes_count = other.boxes_count;
        // null all variables
        // copy other
    }

    linked_stack(linked_stack&& other) noexcept
        : first(other.first), boxes_count(other.boxes_count)
    {
        other.first = nullptr;
        other.boxes_count = 0;
    }

    // canonical form
    linked_stack& operator=(const linked_stack& other)
    {

        box<T>* copy = clone_chain(other.first);
        std::size_t count = other.boxes_count;
        free_chain(first);
        first = copy;
        boxes_count = count;
        return *this;

    }

    // copy-and-swap idiom
    linked_stack& operator=(linked_stack other)
    {
        swap(other);
        return *this;
    }

    ~linked_stack()
    {
        while(first)
            pop();
    }

    void swap(linked_stack& other)
    {
        std::swap(first, other.first);
        std::swap(boxes_count, other.boxes_count);
    }

    // strong exception safety guarantee
    void push(const T& value)
    {
        first = alloc.allocate(value, first);
        ++boxes_count;
    }

    void pop()
    {
        if(empty())
            throw std::exception();

        box<T>* temp = first;
        first = first -> next;
        alloc.free(temp);

        --boxes_count;
    }

    T& top()
    {
        if(empty())
            throw std::exception();
        //assert(!empty());
        return first -> value;
    }

    const T& top() const
    {
        if(empty())
        throw std::exception();
        //assert(!empty());
        return first -> value;
    }


    bool empty() const
    { return size() == 0; }

    std::size_t size() const
    { return boxes_count; }

    const AllocatorType& get_allocator() const
    {
        return alloc;
    }

private:
    void pop_interval()
    {
        while(first)
        {
            box<T>* temp = first;
            first = first -> next;
            alloc.free(temp);
        }
    }

};
