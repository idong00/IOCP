#include <iostream>
#include <boost/bind.hpp>
#include <vector>
#include <algorithm>

void print(std::ostream* os, int i)
{
	*os << i << '\n';
}

void Print(int a, int b)
{
	printf("%i, %i\r\n", a, b);
}

int main()
{
	// placeholder
	//boost::bind(Print, _1, _2)(1, 3);
	std::vector<int> v{ 1, 3, 2 };
	std::for_each(v.begin(), v.end(), boost::bind(print, &std::cout, _1));
	return 0;
}