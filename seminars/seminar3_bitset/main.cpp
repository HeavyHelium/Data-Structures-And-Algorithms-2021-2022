#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

// change to 1 to compile StaticBitset
#if 1

class StaticBitset
{
	const int capacity;// number of uint32_ts allocated
	uint32_t* data;
	const int count;
public:
	StaticBitset(int count, bool value)
        : capacity(determine_size_as_int(count)), data(new uint32_t[capacity]), count(count)
    {
        std::cout << "cap: " << capacity << std::endl;
		if (value)
            memset(data, 255, capacity * 4);// 255 = 2^8 - 1, hence all ones in an 8-bit byte
		else
            memset(data, 0, capacity * 4);
	}

	/// Set value at index to true
	void set(int index)
	{
	    int pos = index / 32;
	    int current_bit_l_r = index - 32 * pos;
		uint32_t temp = 1 << (32 - 1 - current_bit_l_r) ;
		data[pos] = data[pos] | temp;
	}

	/// Set value at @index to false
	void clear(int index)
	{
		int pos = index / 32;
	    int current_bit_l_r = index - 32 * pos;
		uint32_t temp = 1 << (32 - 1 - current_bit_l_r) ;
		data[pos] = (data[pos] & (~temp));
	}

	/// Get value at @index
	bool get(int index) const
	{
		int pos = index / 32;
	    int current_bit_l_r = index - 32 * pos;
		uint32_t temp = 1 << (32 - 1 - current_bit_l_r) ;
        return data[pos] & temp;
	}

	/// Get number of bits
	int size() const { return count; }

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
	    if(count == 0) return 0;
	    int times = count / 32;
	    if(count % 32)
            return times + 1;
        return times;
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
		{ // read
			v[c] = true;
			++setCount;
		}

	for(int i = 0; i < v.size(); ++i)
       std::cout << std::boolalpha << v.get(i) << " " << std::endl;
	std::cout << "Number of set bits = " << setCount;

	return 0;
}


#else

int main() {
	return 0;
}

#endif
