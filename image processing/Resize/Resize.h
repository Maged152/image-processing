#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	void Resize(
		const sf::Image& in,
		      sf::Image& out,
		const unsigned int width,
		const unsigned int height,
		ScaleMethod method = ScaleMethod::BILINEAR
	);
}