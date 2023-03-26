#pragma once
#include <type_traits>
#include "SFML/Graphics.hpp"

// sf::Color * num
template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
sf::Color operator*(const sf::Color& c, T num)
{
	sf::Color res{};
	res.r = c.r * num;
	res.g = c.g * num;
	res.b = c.b * num;
	res.a = c.a * num;
	return res;
}

// sum[4] += sf::Color
template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T* operator+=(T sum[4], const sf::Color& c)
{
	sum[0] += c.r;
	sum[1] += c.g;
	sum[2] += c.b;
	sum[3] += c.a;

	return sum;
}
