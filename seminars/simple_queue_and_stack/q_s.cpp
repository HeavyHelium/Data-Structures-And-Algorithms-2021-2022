#include <iostream>
#include <vector>
#include <cstddef>
#include <utility>

template<class T, class container = std::vector<T>>
class stack
{
    container data;
public:
    stack(std::size_t size)
        : data(size)
    {}
    void push(const T& element) { data.push_back(element); }
    void push(T&& element) { data.push_back(std::move(element)); }
    void pop() { data.pop_back(); }
    std::size_t size() const { return data.size(); }
    bool empty() const { return size() == 0; }
    const T& top() const { return data[data.size() - 1]; }
};

template<class T, class Container = std::vector<T>>
class queue
{
    Container data;
    std::size_t m_begin = 0;
    std::size_t m_end = 0;
public:
    void push(const T& element)
    {
        if(m_begin == m_end) { do_resize(); }

        data[m_end] = element;
        m_end = next(m_end);
    }
    void pop() { m_begin = next(m_begin); }
    std::size_t size() const { std::cout << "end" << m_end << std::endl; return (m_end + data.size() - m_begin); }
    bool empty() const { return m_begin == (m_end - 1) % data.size(); }
    const T& front() const { return data[m_begin]; }
    const T& back() const { return data[(m_end - 1) % data.size()]; }

private:
    void do_resize()
    {
        m_end = data.size();
        data.resize(data.size() ? 2 * data.size() : 1);
        for(std::size_t i = 0; i < m_begin; ++i)
            push(data[i]);
    }
    std::size_t next(std::size_t index) { return (index + 1) % data.size(); }
};

int main()
{
    queue<int> q1;
    q1.push(1);
    q1.push(2);
    //q1.pop();
    std::cout << q1.front() << std::endl;
  //  q1.push(3);
  //  q1.push(4);
 //   q1.push(5);
    std::cout << q1.size() << std::endl;

    return 0;
}
