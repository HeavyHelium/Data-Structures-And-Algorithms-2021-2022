#ifndef LIST_WITH_SORTED_INSERTION_HPP
#include "list.hpp"

template<typename T>
class list_with_sorted_insertion : public list<T>
{
public:
	list_with_sorted_insertion() = default;
	list_with_sorted_insertion(const std::initializer_list<T>& lst) 
	{ for (const T& element : lst) insert(element); }

	void insert(const T& data)
	{
		typename list<T>::iterator pos = list<T>::begin();
		for (; pos != list<T>::end() && *pos <= data ; ++pos)
			;
		list<T>::insert(pos, data);
	}
};

#define LIST_WITH_SORTED_INSERTION_HPP
#endif

