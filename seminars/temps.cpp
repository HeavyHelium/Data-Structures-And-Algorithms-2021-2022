#include <iostream>
#include <vector>

template<bool cond, typename T>
struct enable_if
{ typedef T type; };

template<typename T>
struct enable_if<false, T>
{};

template<typename T>
struct is_pointer
{ static constexpr bool value = false; };

template <typename T>
typename enable_if<is_pointer<T>::value, void>::type deepCopy(T& dest, T& source)
{ *dest = *source; }

template <typename T>
struct implements_dereference
// I need a type, which is a templated type by the member function operator* of another templated type
{
    //struct dummy<auto (T::*) >
};
//
//template<typename T, bool DisableB = std::is_fundamental<T>::value>
//struct has_dereference_operator
//{
//private:
//    typedef char no;
//    struct yes { no m[2]; };
//
//    struct ambiguator { char* operator*() { return nullptr; } };
//    struct combined : T, ambiguator { };
//    static combined* make();
//
//    template<typename U, U> struct check_impl;
//    template<typename U>
//    static no check(
//        U*,
//        check_impl<char* (ambiguator::*)(), &U::operator*>* = nullptr
//    );
//    static yes check(...);
//
//public:
//    static bool const value=std::is_same<decltype(check(make())), yes>::value;
//};



template <typename T>
struct ScopedPointer
{
private:
	T *ptr;
public:
	ScopedPointer(const ScopedPointer &) = delete;
	ScopedPointer &operator=(const ScopedPointer &) = delete;

	ScopedPointer(T *ptr)
		: ptr(ptr) {}

	T &operator*()
	{ return *ptr; }

	~ScopedPointer()
	{ delete ptr; }
};



/*
- За прости типове просто трябва да ги копира
- За типове които са указатели трябва да копира стойността на обекта а не на указателя
- За типове които имат operator* трябва да го използва него
*/
// TODO: implement this, without function overloading, use templates and SFINAE



/// partial specialization, where T is T*

class apple
{
public:
   void operator*() {std::cout << "hello\n\n"; }
};

void f() {}

int main()
{
     apple a = apple();
    using MemberVarPtr_95 = void ((apple::*))();
    void ((apple::*ptr))() = &apple::operator*;
 //   (apple::operator*)* x;
    //std::cout << is_pointer<int*>::value << std::endl;
//    std::cout << has_dereference_operator<std::vector<int>::iterator>::value << std::endl;
	{
		ScopedPointer<int> x(new int{100}), y(new int{2});
		std::cout << "x = " << *x << " y = " << *y << std::endl;
		//deepCopy(x, y);
		std::cout << "x = " << *x << " y = " << *y << std::endl;
	}

	{
		float x = 3.14f, y = 17.3f;
		std::cout << "x = " << x << " y = " << y << std::endl;
//		deepCopy(x, y);
		std::cout << "x = " << x << " y = " << y << std::endl;
	}

	{
		double vals[2] = {100.1, 200.2};
		double *x = &vals[0];
		double *y = &vals[1];
		std::cout << "x = " << *x << " y = " << *y << std::endl;
		//deepCopy(x, y);
		std::cout << "x = " << *x << " y = " << *y << std::endl;
	}

	/* Expected output:
	 * x = 100 y = 2
	 * x = 2 y = 2
	 * x = 3.14 y = 17.3
	 * x = 17.3 y = 17.3
	 * x = 100.1 y = 200.2
	 * x = 200.2 y = 200.2
	 */

	return 0;
}
