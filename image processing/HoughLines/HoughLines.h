#pragma once
#include "SFML/Graphics.hpp"
#include <numbers>
namespace qlm
{
	// line in the polar form
	struct LinesPolar
	{
		int radius;
		int angle;
	};
	void HoughLines(const sf::Image& in, std::vector<LinesPolar>& lines, 
					float  rho, float theta_step, int threshold,
					double min_theta = 0, double max_theta = std::numbers::pi);
}