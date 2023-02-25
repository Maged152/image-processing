#pragma once
#include "SFML/Graphics.hpp"
#include <numbers>
namespace qlm
{
	// line in the polar form
	struct LinesPolar
	{
	public:
		int num_lines;
		std::vector<int> radius;
		std::vector<int> angle;
	public:
		LinesPolar(): num_lines{0},radius{0},angle{0}
		{}
	
	};
	void HoughLines(const sf::Image& in, LinesPolar lines, float  rho, float theta,
					int threshold, double min_theta = 0, double max_theta = std::numbers::pi
		);
}