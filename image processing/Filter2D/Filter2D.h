#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	void Filter2D(
		const sf::Image& in,
		      sf::Image& out,
		const Kernel& kernel,
		      BORDER border = BORDER::BORDER_CONSTANT,
		      int border_const = 0
	);
}