#include <iostream>
#include <functional>

int fun(int a, int b, int c, int d)
{
	std::cout << "------------------------------- fun ------------------------------ "<< std::endl;
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
	std::cout << d << std::endl;
	return 0;
}

class CCaller
{
public:
	int operator()(int a, int b, int c, int d)
	{
		std::cout << "------------------------------------ CCaller ---------------------------" << std::endl;
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << c << std::endl;
		std::cout << d << std::endl;
		return 0;
	}
};

class CCaller2
{
public:
	int operator()(int a, int b, int c, int d)
	{
		std::cout << "-------------------------- CCaller2 ------------------------------" << std::endl;
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << c << std::endl;
		std::cout << d << std::endl;
		return 0;
	}

	int a = 1;
	int b = 10;
	int c = 100;
};
int main()
{
	{
		CCaller Caller;
		std::function<int(int, int, int, int)> f;

		f = [](int a, int b, int c, int d) -> int
		{
			std::cout << "-------------------- f ------------------" << std::endl;
			std::cout << a << std::endl;
			std::cout << b << std::endl;
			std::cout << c << std::endl;
			std::cout << d << std::endl;
			return 0;
		};
		f(1, 2, 3, 4);

		f = Caller;
		f(10, 20, 30, 40);

		f = fun;
		f(100, 200, 300, 400);
	}

	{
		CCaller2 Caller;
		std::function<int(int, int, int, int)> f;

		f = Caller;
		f(10, 20, 30, 40);
	}

	return 0;
}