#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	sf::Image SepFilter2D(
		const sf::Image& in,
		const Kernel1D& x_kernel,
		const Kernel1D& y_kernel,
		const BORDER border_type = BORDER::BORDER_CONSTANT,
		const int border_value = 0
	);

}
