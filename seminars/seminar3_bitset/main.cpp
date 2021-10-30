#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>
#include <cassert>

// change to 1 to compile StaticBitset
#if 1

class StaticBitset
{
	int m_capacity;// number of bytes allocated
	uint32_t* data;
	int count;
public:
	StaticBitset(int count, bool value)
        : m_capacity(determine_size_as_int(count)),
          data(m_capacity ? new uint32_t[m_capacity / 4] : nullptr),
          count(count)
    {
		if (value)
            std::memset(data, 255, m_capacity);
		else
            std::memset(data, 0, m_capacity);
	}

    // Rule of five
	StaticBitset(const StaticBitset& other)
        : m_capacity(other.m_capacity),
          data(new uint32_t[m_capacity]),
          count(other.count)
	{
	    std::memcpy(data, other.data, other.m_capacity);
	}

	StaticBitset(StaticBitset&& other) noexcept
        : m_capacity(other.m_capacity),
          data(other.data),
          count(other.count)
	{
	    other.count = 0;
	    other.m_capacity = 0;
	    other.data = nullptr;
	}
	// copy-and-swap
	StaticBitset& operator=(StaticBitset other)
	{
	    std::swap(data, other.data);
	    std::swap(m_capacity, other.m_capacity);
	    std::swap(count, other.count);
	    return *this;
	}
	~StaticBitset() { delete data; }

	/// Set value at index to true
	void set(int index)
	{
	  	assert(index >= 0 && index < count);
	  	int pos = index / 32;
			uint32_t temp = 1 << calculate_shift(pos, index);
			data[pos] = data[pos] | temp;
	}

	/// Set value at @index to false
	void clear(int index)
	{
    	assert(index >= 0 && index < count);
    	int pos = index / 32;
			uint32_t temp = 1 << calculate_shift(pos, index);
			data[pos] = (data[pos] & (~temp));
	}

	/// Get value at @index
	bool get(int index) const
	{
      assert(index >= 0&& index < count);
      int pos = index / 32;
		  uint32_t temp = 1 << calculate_shift(pos, index);
      return data[pos] & temp;
	}

	/// Get number of bits
	int size() const { return count; }
	int capacity() const { return m_capacity; } // as bytes

	/// Proxy of 1 bit of the StaticBitset
	struct proxy
	{
	private:
		StaticBitset* bitset;
		const int index;
		friend struct StaticBitset;
		proxy(StaticBitset* owner, int index)
            : bitset(owner), index(index)
		{}
	public:
		proxy& operator=(bool value)
		{
			if(value)
                bitset->set(index);
            else
                bitset->clear(index);

			return *this;
		}

		operator bool() const
		{ return bitset->get(index); }
	};

	/// Read/write access to a specific bit in the set
	proxy operator[](int index)
	{ return proxy { this, index }; }

	/// Const overload, read only access
	bool operator[](int index) const
	{ return get(index); }

private:
	static int determine_size_as_int(int count)
	{
	    int times = count / 32;
	    if(count % 32)
            return 4 * (times + 1);
        return 4 * times;
	}
	static int calculate_shift(int pos, int index)
	{
	    int current_bit_l_r = index - 32 * pos; // we find where the bit is in the integer (left to right)
        return (31 - current_bit_l_r); // return shift right to left
	}
};

int main()
{
	StaticBitset v(15, false);


	v.clear(5);
	std::cout << v.size() << '\n';

	int setCount = 0;
	for (int c = 0; c < v.size(); c++)
		if (c % 2)
		{
			v[c] = true;
			++setCount;
		}

	StaticBitset copy(std::move(v));
	std::cout << "size: " << v.size() << std::endl;
	for(int i = 0; i < v.size(); ++i)
       std::cout << std::boolalpha << v.get(i) << " " << std::endl;
	std::cout << "Number of set bits = " << setCount << '\n';
	for(int i = 0; i < copy.size(); ++i)
        std::cout << copy[i] << std::endl;

	return 0;
}


#else

int main() {
	return 0;
}

#endif
