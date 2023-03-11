#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"
#include <numbers>
namespace qlm
{
	void HoughLines(const sf::Image& in, std::vector<LinePolar>& lines, 
					float  rho, float theta_step, int threshold,
					double min_theta = 0, double max_theta = std::numbers::pi);
}