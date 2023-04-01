#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"
#include "color_operations.h"

namespace qlm
{
	void Filter2D(
		const sf::Image& in,
		      sf::Image& out,
		const Kernel& kernel,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
		      int border_value = 0
	);
}