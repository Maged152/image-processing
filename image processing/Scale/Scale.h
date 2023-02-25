#pragma once
#include "SFML/Graphics.hpp"

namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	};

	void Scale(const sf::Image& in, sf::Image& out, ScaleMethod method, float scale_x, float scale_y);
} // namespace