
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
		: m_capacity(determine_size_in_bytes(count)),
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
		: m_capacity(determine_size_in_bytes(other.count)),
		data(new uint32_t[m_capacity]),
		count(other.count)
	{
		std::memcpy(data, other.data, m_capacity);
	}

	StaticBitset(StaticBitset&& other)
		: m_capacity(other.m_capacity),
		data(other.data),
		count(other.count)
	{
		other.count = 0;
		other.m_capacity = 0;
		other.data = nullptr;
	}

	StaticBitset& operator=(const StaticBitset& other)
	{
		if (this != &other)
		{
			int other_cnt = determine_size_in_bytes(other.count);// as bytes
			if (m_capacity < other_cnt)
			{
				uint32_t* temp = new uint32_t[other_cnt / 4];
				memcpy(temp, other.data, other_cnt);
				delete[] data;
				data = temp;
				m_capacity = other_cnt;
			}
			// if there is enough space we do not reallocate
			else memcpy(data, other.data, other.m_capacity);
			count = other.count;
		}
		return *this;
	}

	StaticBitset& operator=(StaticBitset&& other)
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
		assert(("Invalid index!", (index >= 0 && index < count)));
		int pos = index / 32;
		uint32_t temp = 1 << calculate_shift(index);
		data[pos] = data[pos] | temp;
	}

	/// Set value at @index to false
	void clear(int index)
	{
		assert(("Invalid index!", (index >= 0 && index < count)));
		int pos = index / 32;
		uint32_t temp = 1 << calculate_shift(index);
		data[pos] = (data[pos] & (~temp));
	}

	/// Get value at @index
	bool get(int index) const
	{
		assert(("Invalid index!", (index >= 0 && index < count)));
		int pos = index / 32;
		uint32_t temp = 1 << calculate_shift(index);
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
			if (value)
				bitset->set(index);
			else
				bitset->clear(index);

			return *this;
		}

		operator bool() const
		{
			return bitset->get(index);
		}
	};

	/// Read/write access to a specific bit in the set
	proxy operator[](int index)
	{
		return proxy{ this, index };
	}

	/// Const overload, read only access
	bool operator[](int index) const
	{
		return get(index);
	}

	friend std::ostream& operator<<(std::ostream& os, const StaticBitset& bitset)
	{
		for (int i = 0; i < bitset.size(); ++i)
			os << bitset.get(i) << " ";
		return os;
	}

private:
	static int determine_size_in_bytes(int count)
	{
		assert(("Size cannot be negative!", count >= 0));
		int times = count / 32;
		if (count % 32)
			return 4 * (times + 1);
		return 4 * times;
	}
	static int calculate_shift(int index)
	{
		int current_bit_l_r = index % 32; // we find where the bit is in the integer (left to right)
		return (31 - current_bit_l_r); // return right to left result
	}
};

int main()
{

	    // construction with fewer bits than 32
	    StaticBitset v1(16, false);

	    assert(v1.capacity() == 4);
	    assert(v1.size() == 16);

	    for(int i = 0; i < 16; ++i)
	        assert(v1[i] == false);

	    v1.set(4);
	    assert(v1[4] == 1);
	    v1.clear(4);
	    assert(v1[4] == 0);

		// construction with exactly 32 bits

		StaticBitset v2(32, true);

	    assert(v2.capacity() == 4);
	    assert(v2.size() == 32);

	    for(int i = 0; i < 32; ++i)
	        assert(v2[i] == true);

	    v2.clear(4);
	    assert(v2[4] == 0);
	    v2.set(4);
	    assert(v2[4] == 1);
	    v2.clear(31);
	    assert(v2[31] == 0);
	    v2.clear(0);
	    assert(v2[0] == 0);

	    // construction with more than 32 bits
	    StaticBitset v3(64, false);
	    assert(v3.capacity() == 8);
	    assert(v3.size() == 64);

	    for(int i = 0; i < 64; ++i)
	        assert(v3[i] == false);

	    v3.clear(4);
	    assert(v3[4] == 0);
	    v3.set(4);
	    assert(v3[4] == 1);
	    v3.set(31);
	    assert(v3[31] == 1);
	    v3.set(0);
	    assert(v3[0] == 1);
	    v3.set(63);
	    assert(v3[63] == 1);
	    v3.set(34);
	    assert(v3[63] == 1);

	    // copy assignment and copy construction work properly

	    StaticBitset v4(v3);
	    v3 = v1;
	    assert(v3.capacity() == 8);
	    assert(v3.size() == v1.size());
	    for(int i = 0; i < v3.size(); ++i)
	        assert(v3[i] == v1[i]);

	    v1 = v4;
	    assert(v4.capacity() == 8);
	    assert(v4.size() == v1.size());
	    for(int i = 0; i < v4.size(); ++i)
	        assert(v1[i] == v4[i]);

	StaticBitset v(1000, false);

	// StaticBitset::proxy p; // Compiler error - proxy is meaningless without bitset

	for (int c = 0; c < v.size(); c++) {
		if (c % 2 == 0) {
			v[c] = true; // write
		}
	}

	int setCount = 0;
	for (int c = 0; c < v.size(); c++) {
		if (v[c]) { // read
			++setCount;
		}
	}

	std::cout << "Number of set bits = " << setCount;

	return 0;
}


#else
int main() {
	return 0;
}
#endif

