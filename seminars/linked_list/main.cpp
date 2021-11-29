#include <iostream>
#include <cstddef>
#include <cassert>
#include <utility>
#include <string>
#include <vector>
#include <list>
#include <initializer_list>


// (doubly)linked lists do not copy because it
// does neither reallocate, nor physically shift

template<class T>
class linked_list
{

public:
    struct Node
    {
        Node* prev = nullptr;
        T value;
        Node* next = nullptr;
    };
    class iterator
    {
        friend class linked_list;
        Node* ptr;
    public:
        iterator(Node* ptr) : ptr(ptr) {}
        iterator& operator++() { ptr = ptr->next; return *this; }
        iterator operator++(int)
        {
            iterator copy(*this);
            ++(*this);
            return copy;
        }
        iterator& operator--() { ptr = ptr->prev; return *this; }
        iterator operator--(int)
        {
            iterator copy(*this);
            --(*this);
            return copy;
        }
        iterator& operator+=(int offset)
        {
            if (offset < 0)
                while (offset < 0)
                {
                    ptr = ptr->prev;
                    ++offset;
                }
            else
                while (offset > 0)
                {
                    ptr = ptr->next;
                    --offset;
                }
            return *this;
        }
        iterator& operator-=(int offset)
        {
            return *this += (-offset);
        }
        iterator operator+(int offset)
        {
            iterator temp(*this);
            return temp += offset;
        }
        iterator operator-(int offset)
        {
            return (*this + (-offset));
        }

        bool operator==(const iterator& other) const
        {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }
        T& operator*() const
        {
            return ptr->value;
        }
        Node* operator->() const
        {
            return ptr;
        }
        // bool

    };

    class const_iterator
    {
        friend class linked_list;
        const Node* ptr;
    public:
        const_iterator(const Node* ptr) : ptr(ptr) {}
        const_iterator& operator++() { ptr = ptr->next; return *this; }
        const_iterator operator++(int)
        {
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const_iterator& operator--() { ptr = ptr->prev; return *this; }
        const_iterator operator--(int)
        {
            const_iterator copy(*this);
            --(*this);
            return copy;
        }
        const_iterator& operator+=(int offset)
        {
            if (offset < 0)
                while (offset < 0)
                {
                    ptr = ptr->prev;
                    ++offset;
                }
            else
                while (offset > 0)
                {
                    ptr = ptr->next;
                    --offset;
                }
            return *this;
        }
        const_iterator& operator-=(int offset)
        {
            return *this += (-offset);
        }
        const_iterator operator+(int offset)
        {
            const_iterator temp(*this);
            return temp += offset;
        }
        const_iterator operator-(int offset)
        {
            return (*this + (-offset));
        }

        bool operator==(const const_iterator& other) const
        {
            return ptr == other.ptr;
        }
        bool operator!=(const const_iterator& other) const
        {
            return !(*this == other);
        }
        const T& operator*() const
        {
            return ptr->value;
        }
        Node* operator->() const
        {
            return ptr;
        }
        // bool

    };


public:

    linked_list() { dummy.prev = dummy.next = &dummy; }

    linked_list(std::size_t count, const T& value = T())
        : linked_list()
    {
        while (count--)
            push_back(value);
    }

    linked_list(std::initializer_list<T> init)
        : linked_list()
    {
        for (const T& elem : init)
            push_back(elem);
    }

    linked_list(const linked_list& other)
        : linked_list()
    {
        try
        {
            for (const_iterator i = other.begin(); i != other.end(); ++i)
                push_back(*i);
        }
        catch (...)
        {
            while (!empty()) pop_back();
        }
    }

    linked_list(linked_list&& other) noexcept
        : linked_list()
    {
        std::swap(dummy, other.dummy);
    }

    linked_list& operator=(linked_list other)
    {
        std::swap(other.dummy, dummy);
        std::swap(m_size, other.m_size);
        return *this;
    }

    void push_front(const T& element)
    {
        insert_before(dummy.next, element);
        ++m_size;
    }
    void push_back(const T& element)
    {
        insert_before(&dummy, element);
        ++m_size;
    }

    void push_front(T&& element)
    {
        insert_before(dummy.next, std::move(element));
        ++m_size;
    }
    void push_back(T&& element)
    {
        insert_before(&dummy, std::move(element));
        ++m_size;
    }

    template< class... Args >
    void emplace_back(Args&&... args)
    {
        insert_before(&dummy, T(std::forward<Args>(args)...));
        ++m_size;
    }

    template< class... Args >
    void emplace_front(Args&&... args)
    {
        insert_before(dummy.next, T(std::forward<Args>(args)...));
        ++m_size;
    }

    void pop_back()
    {
        assert(m_size > 0);
        erase_at(dummy.prev);
        --m_size;
    }
    void pop_front()
    {
        assert(m_size > 0);
        erase_at(dummy.next);
        --m_size;
    }

    iterator begin() { return { dummy.next }; }
    iterator end() { return &dummy; }

    const_iterator begin() const { return { dummy.next }; }
    const_iterator end() const { return const_iterator{ &dummy }; }

    void insert(iterator pos, const T& value)
    {
        insert_before(pos.ptr, value);
        ++m_size;
    }

    void insert(iterator pos, T&& value)
    {
        insert_before(pos.ptr, std::move(value));
        ++m_size;
    }

    void splice(iterator pos, linked_list& other)
    {
        pos = pos->prev;
        Node* temp = pos.ptr->next;
        temp->prev = other.dummy.prev;
        other.dummy.prev->next = temp;
        pos->next = other.dummy.next;
        other.dummy.next->prev = pos.ptr;
        m_size += other.m_size;
        other.m_size = 0;
        other.dummy.next = other.dummy.next = nullptr;
    }

    void splice(iterator pos, linked_list&& other)
    {
        pos = pos->prev;
        Node* temp = pos.ptr->next;
        temp->prev = other.dummy.prev;
        other.dummy.prev->next = temp;
        pos->next = other.dummy.next;
        other.dummy.next->prev = pos.ptr;
        m_size += other.m_size;
        other.m_size = 0;
        other.dummy.next = other.dummy.next = nullptr;
    }


    void merge(linked_list& other)
    {
        Node* current = &dummy;
        iterator i1 = dummy.next;
        iterator i2 = other.dummy.next;
        while (i1 != end() && i2 != other.end())
        {
            //std::cout << i1.ptr->value << " i2 :" << i2.ptr->value << std::endl;
            Node* choice = (i1->value < i2->value) ? (i1++).ptr : (i2++).ptr;
            current->next = choice;
            choice->prev = current;
            current = current->next;
        }
        if (i1 != end())
        {
            while (i1 != end())
            {
                current->next = (i1++).ptr;
                current->next->prev = current;
                current = current->next;
            }
        }
        else if (i2 != other.end())
        {
            while (i2 != other.end())
            {
                //std::cout << i2->value << std::endl;
               // std::cout << current->prev->value << std::endl;
                current->next = (i2++).ptr;
                current->next->prev = current;
                current = current->next;
            }
        }
        current->next = &dummy;
        dummy.prev = current;
        m_size += other.m_size;
        other.dummy.next = other.dummy.prev = nullptr;
        other.m_size = 0;
    }

    void merge(linked_list&& other)
    {
        Node* current = &dummy;
        iterator i1 = dummy.next;
        iterator i2 = other.dummy.next;
        while (i1 != end() && i2 != other.end())
        {
            std::cout << i1.ptr->value << " i2 :" << i2.ptr->value << std::endl;
            Node* choice = (i1->value < i2->value) ? (i1++).ptr : (i2++).ptr;
            current->next = choice;
            choice->prev = current;
            current = current->next;
        }
        if (i1 != end())
        {
            while (i1 != end())
            {
                current->next = (i1++).ptr;
                current->next->prev = current;
                current = current->next;
            }
        }
        else if (i2 != other.end())
        {
            while (i2 != other.end())
            {
                std::cout << i2->value << std::endl;
                std::cout << current->prev->value << std::endl;
                current->next = (i2++).ptr;
                current->next->prev = current;
                current = current->next;
            }
        }
        current->next = &dummy;
        dummy.prev = current;
        m_size += other.m_size;
        other.dummy.next = other.dummy.prev = nullptr;
        other.m_size = 0;
    }

    void unique()
    {
        for (iterator i = begin() + 1; i != end(); ++i)
            if (i->value == (i - 1)->value)
                erase(i);
    }

    void erase(iterator pos)
    {
        --m_size;
        erase_at(pos.ptr);
    }

    void clear()
    {
        while (!empty())
            pop_back();
    }

    const T& front() const
    {
        return dummy.next->value;
    }
    const T& back() const
    {
        return dummy.prev->value;
    }
    T& front()
    {
        return dummy.next->value;
    }
    T& back()
    {
        return dummy.prev->value;
    }
    bool empty() const
    {
        return (m_size == 0);
    }
    std::size_t size() const
    {
        return m_size;
    }

    ~linked_list()
    {
        while (!empty()) pop_back();/* std::cout << size() << "dtor\n";*/
    }
private:
    static void insert_before(Node* pos, const T& element)
    {
        Node* new_node = new Node{ pos->prev, element, pos };
        pos->prev->next = new_node;
        pos->prev = new_node;
    }
    static void insert_before(Node* pos, T&& element)
    {
        Node* new_node = new Node{ pos->prev, std::move(element), pos };
        pos->prev->next = new_node;
        pos->prev = new_node;
    }
    static void erase_at(Node* pos)
    {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        delete pos;
    }

    Node dummy;// requires default ctor
    std::size_t m_size = 0;
};
int main()
{


    linked_list<int> instance(10, int{});
    assert(instance.size() == 10);

    linked_list<int> copy(instance);
    assert(copy.size() == 10);

    //linked_list<int> assign;
    //assert(assign.size() == 0);
    //assign = copy;

    //assign = copy;

			//assert(assign.size() == copy.size());
			//std::cout << assign.size() << std::endl;

			//linked_list<int> copy(instance);
			//assert(copy.size() == 10);
    /*
    int arr[] = {1, 2, 3, 3, 4, 3, 5, 5, 6, 7, 7};
    std::size_t size = sizeof(arr) / sizeof(int);
    filter_arr(arr, size);

    for(std::size_t i = 0; i < size; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;*/
/*
    linked_list<int>l1{ 1, 3, 3, 5, 7 };
    linked_list<int>l2{ };
    l1.unique();
    for(const auto& elem : l1)
        std::cout << elem << ", ";
    std::cout << std::endl;
    l1.merge(l2);
    std::cout << l1.size() << " , l2: " << l2.size() << std::endl;
    for(const auto& elem : l1)
       std::cout << elem << " ";
    std::cout << "hakunamatata";
//    std::list<int> l1, {1, 2, 3, 4, 5, 6};
//    std::list<int> l2{7, 8, 9};
//    l1.merge(l2);
//    for(auto elem: l1)
//        std::cout << elem << " ";
//    std::cout << std::endl;
//    std::cout << l2.size() << std::endl;
//    for(auto elem: l2)
//        std::cout << elem << " ";
//        std::cout << std::endl;*/

    return 0;
}
