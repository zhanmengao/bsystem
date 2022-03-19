#pragma once
#include<functional>

template<typename T>
size_t GetHashCode(const T& val)
{
	std::hash<T>	hash;
	return hash(val);
}