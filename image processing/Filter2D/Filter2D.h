#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	sf::Image Filter2D(
		const sf::Image& in,
		const Kernel& kernel,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
		      int border_value = 0
	);
}