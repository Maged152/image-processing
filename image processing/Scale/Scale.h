#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	void Scale(const sf::Image& in, sf::Image& out, ScaleMethod method, float scale_x, float scale_y);
} // namespace