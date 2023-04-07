#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	sf::Image Scale(const sf::Image& in, 
		                  ScaleMethod method, 
		                  float scale_x, 
		                  float scale_y);
} // namespace