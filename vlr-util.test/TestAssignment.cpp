#include "pch.h"

class Foo
{
public:
	Foo()
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	~Foo()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	int m_nValue = 42;
};

class Bar
{
public:
	Bar()
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	Bar(int nValue)
		: m_nValue{ nValue }
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	~Bar()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	int m_nValue = 42;
};

int GetValueFromFoo()
{
	Foo oFoo;
	return oFoo.m_nValue;
}

TEST(Assignment, general)
{
	Bar oBar{ GetValueFromFoo() };
}
