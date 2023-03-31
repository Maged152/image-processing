#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	void SepFilter2D(
		const sf::Image& in,
		sf::Image& out,
		const Kernel1D& kernel_x,
		const Kernel1D& kernel_y,
		const BORDER border = BORDER::BORDER_CONSTANT,
		const int border_const = 0
	);

}
