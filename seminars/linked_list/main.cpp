#include <iostream>
#include <cstddef>
#include <cassert>
#include <utility>
#include <string>

// (doubly)linked lists do not copy because it
// does neither reallocate, nor physically shift

/// @brief doubly linked list
template<class T>
class linked_list
{
    struct Node
    {
        Node* prev = nullptr;
        T value;
        Node* next = nullptr;
    };

    Node dummy;// requires default ctor
    std::size_t m_size = 0;
public:
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
            if(offset < 0)
                while(offset < 0)
                {
                    ptr = ptr->prev;
                    ++offset;
                }
            else
                while(offset > 0)
                {
                    ptr = ptr->next;
                    --offset;
                }
            return *this;
        }
        iterator& operator-=(int offset)
        { return *this += (-offset); }
        iterator operator+(int offset)
        {
            iterator temp(*this);
            return temp += offset;
        }
        iterator operator-(int offset)
        { return (*this + (-offset)); }

        bool operator==(const iterator& other) const
        { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const
        { return !(*this == other); }
        T& operator*() const
        { return ptr->value; }
        Node* operator->() const
        { return ptr; }
       // bool

    };
public:

    linked_list() { dummy.prev = dummy.next = &dummy; }

    linked_list(const linked_list& other)
        : m_size(0)
    {
        try
        {
         for(const T& element : other)
             push_back(element);
        }
        catch(...)
        { while(!empty()) pop_back(); }
    }

    linked_list(linked_list&& other) noexcept
        : linked_list()
    { std::swap(dummy, other.dummy); }

    linked_list& operator=(linked_list other)
    {
        std::swap(other.dummy, dummy);
        std::swap(m_size, other.m_size);
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
    void emplace_back( Args&&... args )
    {
        insert_before(&dummy, T(std::forward<Args>(args)...));
        ++m_size;
    }

    template< class... Args >
    void emplace_front( Args&&... args )
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

    iterator begin() { return {dummy.next}; }
    iterator end() { return &dummy; }

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

    void erase(iterator pos)
    {
        ++m_size;
        erase_at(pos.ptr);
    }
    const T& front() const
    { return dummy.next->value; }
    const T& back() const
    { return dummy.prev->data; }
    bool empty() const
    { return (m_size == 0); }
    std::size_t size() const
    { return m_size; }

    ~linked_list()
   { while(!empty()) pop_back(); }
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

};

class apple
{
    double weight = 0;
    std::string color = "beautiful";
public:
    apple(double w, const std::string& c)
        : weight(w), color(c)
    {std::cout << "ctor\n";}
    apple(const apple& other)
    {std::cout << "copy ctor\n";}
    apple& operator=(const apple& other)
    {std::cout << "operator=\n";}
};

int main()
{
    linked_list<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(4);
    l1.push_back(5);
    l1.emplace_front(6);
    l1.insert(l1.begin(), 7);
    std::cout << l1.size() << std::endl;
//    class linked_list<int>::iterator beg = l1.begin();

    std::cout << l1.front() << std::endl;
    for(auto i : l1)
        std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}
