#ifndef QUEUE_HPP_INCLUDED
#define QUEUE_HPP_INCLUDED

#include <cstddef>

// FIFO -- FIRST in FIRST out
// LILO -- LAST in LAST out

template<std::size_t Size>
class queue
{
    T arr[Size];
    std::size_t head;
    std::size_t tail;
public:
    void enqueue(const T& value)
    {

    }
    void dequeue()
    {

    }
    const T& head() const
    {

    }
    T& head()
    {

    }
    std::size_t size() const
    {
        return (m_size + tail - head) & m_size;
    }
    bool empty() const
    {
        return head == tail;
    }
};



#endif // QUEUE_HPP_INCLUDED
