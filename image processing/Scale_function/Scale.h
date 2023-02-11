#pragma once
#include "SFML/Graphics.hpp"

enum class ScaleMethod
{
	NEAREST_NEIGHBOR,
	BILINEAR,
	BICUBIC
};

sf::Image& Scale(sf::Image& in, sf::Image& out, ScaleMethod method, float scale_x, float scale_y);