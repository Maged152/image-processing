#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"
#include <numbers>


namespace qlm
{
	std::vector<LinePolar> HoughLines(const sf::Image& in,
									  float  rho, float theta_step, int threshold,
									  double min_theta = 0, double max_theta = std::numbers::pi);
}