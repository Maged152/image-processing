#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	sf::Image Resize(
		const sf::Image& in,
		const unsigned int width,
		const unsigned int height,
		      ScaleMethod method = ScaleMethod::BILINEAR
	);
}