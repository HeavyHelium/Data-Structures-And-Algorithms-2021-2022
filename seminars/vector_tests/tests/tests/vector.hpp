#pragma once

//#include "mem_inspect.hpp"
#include <cassert>


// if you comment out the next line, the tests will run on std::vector instead
// Make sure all tests pass on std::vector before trying to test your implementation
#define USER_VECTOR

/// Place your implementation below
/// The tests are testing not only interface but implementation details. For this to work allocation and deallocation happen in a specific way:
/// You must replace all new/delete with the following
/// T *ptr = new T[count]; -> NEW(T, count);
/// delete[] ptr; -> DELETE(ptr);

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>
#include <new>

template <typename T>
class vector {

	static constexpr int expansion_factor = 2;
	T* array = nullptr;
	int count = 0;
	int cap = 0;


public:

	class const_iterator
	{
		const T* ptr;
	public:
		const_iterator(T* ptr) : ptr(ptr) {}
		const_iterator& operator++() { ++ptr; return *this; }
		const_iterator operator++(int) { const_iterator copy(*this); ++(*this); return copy; }
		const_iterator& operator--() { --ptr; return *this; }
		const_iterator operator--(int) { const_iterator copy(*this); --(*this); return copy; }

		const_iterator& operator+=(int i)
		{
			ptr += i;
			return *this;
		}

		const_iterator operator+(int i) const
		{
			const_iterator temp(*this);
			return (temp += i);
		}

		const_iterator& operator-=(int i)
		{
			ptr += i;
			return *this;
		}

		const_iterator operator-(int i) const
		{
			const_iterator temp(*this);
			return (temp -= i);
		}

		int operator-(const const_iterator& other)   const
		{
			return ptr - other.ptr;
		}

		const T& operator*()  const { return *ptr; }
		const T* operator->() const { return  ptr; }

		bool operator==(const const_iterator& other) const
		{
			return ptr == other.ptr;
		}
		bool operator!=(const const_iterator& other) const
		{
			return !((*this) == other);
		}

		bool operator<(const const_iterator& other)  const
		{
			return ptr < other.ptr;
		}
		bool operator>=(const const_iterator& other) const
		{
			return !(*this < other);
		}

		bool operator>(const const_iterator& other)  const
		{
			return other < *this;
		}
		bool operator<=(const const_iterator& other) const
		{
			return !(*this < other);
		}
	};

	class iterator
	{
		T* ptr;
	public:
		iterator(T* ptr) : ptr(ptr) {}
		iterator& operator++() { ++ptr; return *this; }
		iterator operator++(int) { iterator copy(*this);  ++(*this); return copy; }
		iterator& operator--() { --ptr; return *this; }
		iterator operator--(int) { iterator copy(*this);  --(*this); return copy; }

		iterator& operator+=(int i)
		{
			ptr += i;
			return *this;
		}

		iterator operator+(int i) const
		{
			iterator temp(*this);
			return (temp += i);
		}

		iterator& operator-=(int i)
		{
			ptr += i;
			return *this;
		}

		iterator operator-(int i) const
		{
			return (iterator(ptr - i));
		}

		int operator-(const iterator& other)   const
		{
			return ptr - other.ptr;
		}

		T& operator*() { return *ptr; }
		const T& operator*()  const { return *ptr; }

		T* operator->() { return  ptr; }
		const T* operator->() const { return  ptr; }

		bool operator==(const iterator& other) const
		{
			return ptr == other.ptr;
		}
		bool operator!=(const iterator& other) const
		{
			return !((*this) == other);
		}

		bool operator<(const iterator& other)  const
		{
			return ptr < other.ptr;
		}
		bool operator>=(const iterator& other) const
		{
			return !(*this < other);
		}

		bool operator>(const iterator& other)  const
		{
			return other < *this;
		}
		bool operator<=(const iterator& other) const
		{
			return !(*this < other);
		}

		operator const_iterator() const { return const_iterator(ptr); }
	};

	class const_reverse_iterator
	{
		const T* ptr;
	public:
		const_reverse_iterator(T* ptr) : ptr(ptr) {}
		const_reverse_iterator& operator++() { --ptr; return *this; }
		const_reverse_iterator operator++(int) { const_reverse_iterator copy(*this); ++(*this); return copy; }
		const_reverse_iterator& operator--() { ++ptr; return *this; }
		const_reverse_iterator operator--(int) { const_reverse_iterator copy(*this); --(*this); return copy; }

		const_reverse_iterator& operator+=(int i) const
		{
			ptr -= i;
			return *this;
		}

		const_reverse_iterator operator+(int i) const
		{
			const_reverse_iterator temp(*this);
			return temp += i;
		}

		const_reverse_iterator& operator-=(int i) const
		{
			ptr += i;
			return *this;
		}

		const_reverse_iterator operator-(int i) const
		{
			const_reverse_iterator temp(*this);
			return temp -= i;
		}

		int operator-(const const_reverse_iterator& other) const
		{ return other.ptr - ptr; }

		const T& operator*()  const { return *ptr; }
		const T* operator->() const { return  ptr; }

		bool operator==(const const_reverse_iterator& other) const
		{ return ptr == other.ptr; }

		bool operator!=(const const_reverse_iterator& other) const
		{ return !((*this) == other); }

		bool operator<(const const_reverse_iterator& other)  const
		{ return ptr > other.ptr; }
		
		bool operator>=(const const_reverse_iterator& other) const
		{ return !(*this < other); }

		bool operator>(const const_reverse_iterator& other)  const
		{ return other < *this; }
		
		bool operator<=(const const_reverse_iterator& other) const
		{ return !(*this > other); }
	};

	class reverse_iterator
	{
		T* ptr;
	public:
		reverse_iterator(T* ptr) : ptr(ptr) {}
		reverse_iterator& operator++()   
		{ --ptr; return *this; }
		
		reverse_iterator operator++(int) 
		{ reverse_iterator copy(*this);  ++(*this); return copy; }
		
		reverse_iterator& operator--() 
		{ ++ptr; return *this; }
		
		reverse_iterator operator--(int) 
		{ reverse_iterator copy(*this);  --(*this); return copy; }

		reverse_iterator operator+(int i) const
		{ return reverse_iterator(ptr - i); }

		reverse_iterator operator-(int i) const
		{ return reverse_iterator(ptr + i); }

		int operator-(const reverse_iterator& other) const
		{ return other.ptr - ptr; }

		T& operator*() { return *ptr; }
		const T& operator*()  const { return *ptr; }

		T* operator->() { return  ptr; }
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
		{ return !(*this > other); }
		
		operator const_reverse_iterator() const 
		{ return const_reverse_iterator(ptr); }
	};

	vector() = default;
	vector(int cnt, const T& defaultElement)
	{
		resize(cnt, defaultElement);
	}

	vector(int size)
		: vector(size, T())
	{}

	vector(const vector& other)
	{
		resize(other.count);
		for (int i = 0; i < other.count; ++i)
		{
			try
			{
				array[i] = other.array[i];
			}
			catch (...) { deallocate_memory(array, i, cap); }
		}

	}

	vector(vector&& other) noexcept
	{
		/// (*this) is a null object,
		/// as its members have default values
		std::swap(array, other.array);
		std::swap(count, other.count);
		std::swap(cap, other.cap);
	}

	vector& operator=(const vector& other)
	{
		if (this != &other)
		{
			reserve(other.count);
			int temp = count;
			int first_limit = std::min(count, other.count);
			for (int i = 0; i < first_limit; ++i)
				array[i] = other[i];
			/// in case there are elements to be constructed in-place
			for (int i = first_limit; i < other.count; ++i)
				new(array + i) T(other[i]);
			count = other.count;
			// if now array is smaller than before
			// we call the destructors of unused elements
			for (int i = count; count < temp; ++i)
				array[i].~T();

		}
		return *this;
	}

	vector& operator=(vector&& other) noexcept
	{
		std::swap(array, other.array);
		std::swap(count, other.count);
		std::swap(cap, other.cap);
	}

	~vector() { deallocate_memory(array, count, cap); }

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
	{ return at(count - 1); }

	T& back()
	{ return at(count - 1); }

	const T& front() const
	{ return at(0); }

	T& front()
	{ return at(0); }

	void push_back(const T& element)
	{
		check_resize();
		new(array + count) T(element);
		++count;
	}

	void push_back(T&& element)
	{
		check_resize();
		new(array + count) T(std::move(element));
		++count;
	}
	
	template<typename...Args>
	void emplace_back(Args&&... args)
	{
		check_resize();
		new(array + count) T(std::forward<Args>(args)...);
		++count;
	}

	void pop_back()
	{
		array[count - 1].~T();
		--count;
	}

	int size() const { return count; }

	int capacity() const { return cap; }

	void resize(int new_size)
	{
		/// we change the size, therefore we are going
		/// to add dafault-constructed elements if necessary 
		if (new_size > cap)
			do_resize(new_size, T());
		else
			for (int i = new_size; i < count; ++i)
				array[i].~T();
		count = new_size;
	}

	void resize(int new_size, const T& value)
	{
		if (new_size > cap)
			do_resize(new_size, value);
		else
			for (int i = new_size; i < count; ++i)
				array[i].~T();
		count = new_size;
	}

	void reserve(int new_capacity)
	{
		if (new_capacity <= cap)
			return;
		else
			do_resize(new_capacity);
	}

	void shrink_to_fit()
	{
		if (cap == count) return;
		do_resize(count);
	}

	void clear() 
	{ 
		for (int i = 0; i < count; ++i)
			array[i].~T();
		count = 0; 
	}

	iterator begin()
	{ return iterator(array); }
	
	iterator end()
	{ return iterator(array + count); }

	const_iterator begin() const
	{ return const_iterator(array); }
	
	const_iterator end()   const
	{ return const_iterator(array + count); }

	reverse_iterator rbegin()
	{ return reverse_iterator(array + count - 1); }
	
	reverse_iterator rend()
	{ return reverse_iterator(array - 1); }

	const_reverse_iterator rbegin() const
	{ return const_reverse_iterator(array + count - 1); }

	const_reverse_iterator rend() const
	{ return const_reverse_iterator(array - 1); }


	void insert(iterator pos, const T& value)
	{
		if (pos < begin() || pos > end())
			throw std::out_of_range("iterator must be >= begin and <= end");
		int offset = pos - begin();
		push_back(T());// just so there is a constructed new element
		--count;
		pos = begin() + offset;// in case of reallocation
		for (iterator i = end(); i > pos; --i)
		{
			try {
				*i = *(i - 1);
			}
			catch (...) { array[count].~T(); throw; }
		}
		*pos = value;
		++count;
	}

	void erase(iterator pos)
	{
		if (pos == end() - 1)
		{
			pop_back();
			return;
		}
		iterator last = end();
		for (iterator i = pos; i < last - 1; ++i)
			*i = *(i + 1);
		--count;
		// now we call the dtor of the last element(the one that is not in the array now)
		array[count].~T();
	}

	void erase(iterator first, iterator last)
	{
		iterator e = end();
		iterator read = last;
		for (iterator i = first; read < e; ++i)
		{
			*i = *(read++);
		}
		int temp = count;
		count = count - (last - first);
		// now we call the dtors of the elements in the end(the ones not in the array now)
		for (int i = count; i < temp; ++i)
			array[i].~T();
	}

	bool empty() const { return count == 0; }


private:
	void do_resize(int new_capacity)
	{
		T* temp = new_capacity ? static_cast<T*>(::operator new(new_capacity * sizeof(T))) : nullptr;
		if (temp)
		{
			int limit = std::min(new_capacity, count);

			for (int i = 0; i < limit; ++i)
			{
				try { new (temp + i) T(array[i]); }
				catch (...) { deallocate_memory(temp, i, new_capacity); throw; }
			}
		}
		deallocate_memory(array, count, cap);
		array = temp;
		cap = new_capacity;
	}

	void do_resize(int new_capacity, const T& value)
	{
		T* temp = new_capacity ? static_cast<T*>(::operator new(new_capacity * sizeof(T))) : nullptr;
		if (temp)
		{
			int limit = std::min(new_capacity, count);

			for (int i = 0; i < limit; ++i)
			{
				try { new (temp + i) T(array[i]); }
				catch (...) { deallocate_memory(temp, i, new_capacity); throw; }
			}

			for (int i = limit; i < new_capacity; ++i)
			{
				try
				{
					new (temp + i) T(value);
				}
				catch (...) { deallocate_memory(temp, i, new_capacity); throw; }
			}
		}
		deallocate_memory(array, count, cap);
		array = temp;
		cap = new_capacity;
	}

	void check_resize()
	{
		assert(count <= cap && "Size is more than capacity");
		if (count + 1 >= cap)
			do_resize(cap ? expansion_factor * cap : 1);
	}
	/// deallocates arrays whose elements have been constructed with placement new
	/// the array itself has been allocated with ::operator new
	/// so ::operator delete is called
	static void deallocate_memory(T* arr, int cnt, int capacity)
	{
		for (int i = 0; i < cnt; ++i)
			arr[i].~T();
		::operator delete(arr, capacity * sizeof(T));
	}
};
