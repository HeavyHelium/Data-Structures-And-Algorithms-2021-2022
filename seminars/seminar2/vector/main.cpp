#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>

template <typename T>
class vector {

	static constexpr int expansion_factor = 2;
	// static constexpr int initial_capacity = 16;
	T* array = nullptr;
	int count = 0;
	int cap = 0;
	// using iterator = T*;
	// using const_iterator = const T*;

public:


	class const_iterator
	{
	    const T* ptr;
	public:
		const_iterator(T* ptr) : ptr(ptr) {}
		const_iterator& operator++()    { ++ptr; return *this; }
		const_iterator operator++(int)  { const_iterator copy(*this);  ++ptr; return copy; }
		const_iterator& operator--()    { --ptr; return *this; }
		const_iterator operator--(int)  { const_iterator copy(*this);  --ptr; return copy; }

		const_iterator operator+(int i) const
		{ return const_iterator(ptr + i); }

		const_iterator operator-(int i) const
		{ return const_iterator(ptr - i); }

		int operator-(const const_iterator& other)   const
		{ return ptr - other.ptr; }

		const T& operator*()        { return *ptr; }
		const T& operator*()  const { return *ptr; }

		const T* operator->()       { return  ptr; }
		const T* operator->() const { return  ptr; }

		bool operator==(const const_iterator& other) const
		{ return ptr == other.ptr; }
		bool operator!=(const const_iterator& other) const
		{ return !((*this) == other); }

		bool operator<(const const_iterator& other)  const
		{ return ptr < other.ptr; }
		bool operator>=(const const_iterator& other) const
		{ return !(*this < other); }

		bool operator>(const const_iterator& other)  const
		{ return other < *this; }
		bool operator<=(const const_iterator& other) const
		{ return !(*this < other); }
	};

	class iterator
	{
		T* ptr;
	public:
		iterator(T* ptr) : ptr(ptr) {}
		iterator& operator++()    { ++ptr; return *this; }
		iterator operator++(int) { iterator copy(*this);  ++ptr; return copy; }
		iterator& operator--()    { --ptr; return *this; }
		iterator operator--(int) { iterator copy(*this);  --ptr; return copy; }

		iterator operator+(int i) const
		{ return iterator(ptr + i); }

		iterator operator-(int i) const
		{ return iterator(ptr - i); }

		int operator-(const iterator& other)   const
		{ return ptr - other.ptr; }

		T& operator*()              { return *ptr; }
		const T& operator*()  const { return *ptr; }

		T* operator->()             { return  ptr; }
		const T* operator->() const { return  ptr; }

		bool operator==(const iterator& other) const
		{ return ptr == other.ptr; }
		bool operator!=(const iterator& other) const
		{ return !((*this) == other); }

		bool operator<(const iterator& other)  const
		{ return ptr < other.ptr; }
		bool operator>=(const iterator& other) const
		{ return !(*this < other); }

		bool operator>(const iterator& other)  const
		{ return other < *this; }
		bool operator<=(const iterator& other) const
		{ return !(*this < other); }

		operator const_iterator() const { return const_iterator(ptr); }
	};


	class reverse_iterator
	{
	    T* ptr;
	public:
		reverse_iterator(T* ptr) : ptr(ptr) { /*std::cout << std::endl << "gosho" << *ptr << std::endl;*/ }
		reverse_iterator& operator++()    { --ptr; return *this; }
		reverse_iterator operator++(int)  { reverse_iterator copy(*this);  ++(*this); return copy; }
		reverse_iterator& operator--()    { ++ptr; return *this; }
		reverse_iterator operator--(int)  { reverse_iterator copy(*this);  --(*this); return copy; }

		reverse_iterator operator+(int i) const
		{ return reverse_iterator(ptr - i); }

		reverse_iterator operator-(int i) const
		{ return reverse_iterator(ptr + i); }

		int operator-(const reverse_iterator& other)   const
		{ return other.ptr - ptr; }

		const T& operator*()        { return *ptr; }
		const T& operator*()  const { return *ptr; }

		const T* operator->()       { return  ptr; }
		const T* operator->() const { return  ptr; }

		bool operator==(const reverse_iterator& other) const
		{ return ptr == other.ptr; }
		bool operator!=(const reverse_iterator& other) const
		{ return !((*this) == other); }

		bool operator<(const reverse_iterator& other)  const
		{ return ptr > other.ptr; }
		bool operator>=(const reverse_iterator& other) const
		{ return !(*this < other); }

		bool operator>(const reverse_iterator& other)  const
		{ return other < *this; }
		bool operator<=(const reverse_iterator& other) const
		{ return !(*this < other); }
	};

	vector() = default;
	vector(int cnt, const T& defaultElement)
	{ resize(cnt, defaultElement); }

	vector(int size)
		: array(size ? new T [size] : nullptr), cap(size), count(size)
	{}

	vector(const vector& other)
		: array(other.count ? new T[other.count] : nullptr), count(other.count), cap(other.count)
	{
		try
		{
			for (int i = 0; i < other.count; ++i)
				array[i] = other.array[i];
		}
		catch (...) { delete[] array; throw; }
	}

	vector& operator=(const vector& other)
	{
		if(this != &other)
		{
			reserve(other.count);
			for (int i = 0; i < other.count; ++i)
				array[i] = other[i];
			count = other.count;
		}
			return *this;

	}

	~vector() { delete[] array; }

	const T& operator[](int index) const
	{
		assert(index < count);
		return array[index];
	}

	T& operator[](int index)
	{
		return const_cast<T&>(const_cast<const vector<T>*>(this)->operator[](index));
	}

	const T& at(int index) const
	{
		if (index < 0 || index >= count)
			throw std::out_of_range("index is out of range");
		return array[index];
	}

	T& at(int index)
	{
		return const_cast<T&>(const_cast<const vector<T>*>(this)->at(index));
	}

	const T& back() const
	{ return array[count - 1]; }

	T& back()
	{ return array[count - 1 ]; }

	const T& front() const
	{ return array[0]; }

	T& front()
	{ return array[0]; }

	void push_back(const T& element)
	{
		check_resize();
		array[count] = element;
		++count;
	}

	void pop_back()
	{ --count; }

	int size() const { return count; }

	int capacity() const { return cap; }

	void resize(int new_size)
	{
	    if(new_size > cap)
            do_resize(new_size);
		count = new_size;
	}

	void resize(int new_size, const T& value)
	{
	    if(new_size > cap)
		    do_resize(new_size, value);
        count = new_size;
	}

	void reserve(int new_capacity)
	{
		if (new_capacity <= cap)
			return;
		do_resize(new_capacity);
	}

	void shrink_to_fit()
	{
	    if(cap == count) return;
		do_resize(count);
	}

	void clear() { count = 0; }

	iterator begin()
	{ return array;         }
	iterator end()
	{ return array + count; }

    const_iterator begin() const
	{ return array;         }
	const_iterator end()   const
	{ return array + count; }

    reverse_iterator rbegin() { return reverse_iterator(array + count - 1); }
    reverse_iterator rend() { return reverse_iterator(array - 1); }


	void insert(iterator pos, const T& value)
	{
		if (pos < begin() || pos > end())
			throw std::out_of_range("iterator must be >= begin and <= end");
		int offset = pos - begin();
		check_resize();
		pos = begin() + offset;
        for(iterator i = end(); i > pos; --i)
            *i = *(i - 1);
        *pos = value;
		++count;
	}

	void erase(const iterator& pos)
	{
		iterator last = end();
		for (iterator i = pos; i < last - 1; ++i)
			*i = *(i + 1);
		--count;
	}

	void erase(const iterator& first, const iterator& last)
	{
        iterator e = end();
		iterator read = last;
		for (iterator i = first; read < e; ++i)
			{ *i = *(read++) ;}
		count = count - (last - first);
	}

	bool empty() const { return count == 0; }


private:


	void do_resize(int new_capacity)
	{
		T* temp = new_capacity ? new T[new_capacity] : nullptr;

		int limit = std::min(new_capacity, count);

		try {
			for (int i = 0; i < limit; ++i)
				temp[i] = array[i];
		}
		catch (...) { delete[] temp; throw; }
		T* toDelete = array;
		array = temp;
		cap = new_capacity;
		delete[] toDelete;
	}

	void do_resize(int new_capacity, const T& value)
	{
		T* temp = new_capacity ? new T[new_capacity] : nullptr;

		int limit = std::min(new_capacity, count);

		try {
			for (int i = 0; i < limit; ++i)
				temp[i] = array[i];
			for (int i = limit; i < new_capacity; ++i)
				temp[i] = value;
		}
		catch (...) { delete[] temp; throw; }
		T* toDelete = array;
		array = temp;
		cap = new_capacity;
		delete[] toDelete;
	}

	void check_resize()
	{
		assert(count <= cap && "Size is more than capacity");
		if (count + 1 >= cap)
			do_resize( cap ? expansion_factor * cap : 1 );
	}


};

int main()
{
    vector<int> vec(1, 0);
    std::cout << std::endl;
    //for(int i: vec) std::cout << i << " ";
    //std::cout << std::endl;

    vec.insert(vec.begin(), 7);
    if(vec[0] != 7) std::cout << "false" << std::endl;
    //std::cout << std::endl;
    //for(int i: vec) std::cout << i << " ";
    //std::cout << std::endl;
   // std::cout << "capacity: " << vec.capacity() << " size: " << vec.size() << std::endl;

    vec.insert(vec.begin() + 1, 4);
    std::cout << std::endl;
    for(int i: vec) std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "capacity: " << vec.capacity() << " size: " << vec.size() << std::endl;

    vec.erase(vec.begin(), vec.begin() + 1);
    std::cout << std::endl;
    for(int i: vec) std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "capacity: " << vec.capacity() << " size: " << vec.size() << std::endl;

    std::cout << *vec.rbegin() << " " << *(vec.rend() - 1) << std::endl;
    for(vector<int>::reverse_iterator i = vec.rbegin(); i != vec.rend(); ++i)
      std::cout << *i << " ";
    std::cout << "\n";

/*
    std::vector<int> cvec = { 1, 2, 3, 4, 5 };
    for(int i: cvec) std::cout << i << " ";
    std::cout << std::endl;

    std::vector<int>::reverse_iterator ri = cvec.rbegin();
    std::cout << *ri << std::endl;

    for(std::vector<int>::const_reverse_iterator i = cvec.rbegin(); i < cvec.rend(); ++i)
        std::cout << *i << " ";
    std::cout << std::endl;*/




    return 0;
}
