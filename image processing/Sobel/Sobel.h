#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{
	SobelDerivatives Sobel(
		const sf::Image& in,
		const unsigned int kernel_size,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
		      int border_value = 0
	);

	sf::Image SobelX(
		const sf::Image& in,
		const unsigned int kernel_size,
		BORDER border_type = BORDER::BORDER_CONSTANT,
		int border_value = 0
	);

	sf::Image SobelY(
		const sf::Image& in,
		const unsigned int kernel_size,
		BORDER border_type = BORDER::BORDER_CONSTANT,
		int border_value = 0
	);

	// TODO : GetSobelXKernel(),GetSobelYKernel()
}