#include <iostream>
#include <iomanip>


int reverse(int a) { 
	int res = 0;
	while(a) { 
		res *= 10;
		res += a % 10;
		a /= 10;
	}
	return res;
}

bool is_palindrome(const int a) { 
	return reverse(a) == a;
}


int main() { 
	std::cout << "Please input a number: ";
	int a;
	std::cin >> a;
	std::cout << "The number inputed is ";
	if(!is_palindrome(a)) { 
		std::cout << "NOT ";
	}
	std::cout << "a palindrome." << std::endl;


	return 0;
}
