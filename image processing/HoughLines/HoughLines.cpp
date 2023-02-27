#include "HoughLines.h"
#include <iostream>
void qlm::HoughLines(const sf::Image& in, std::vector<LinesPolar>& lines, float rho, float theta_step, int threshold, double min_theta, double max_theta)
{
	unsigned int width = in.getSize().x;
	unsigned int height = in.getSize().y;

	int max_rho = std::floor(std::sqrt(width * width + height * height));
	int min_rho = -max_rho;

	if (min_theta > max_theta)
	{
		std::cout << " Error max_theta is less than min_theta\n";
		return;
	}
	int numangle = std::floor((max_theta - min_theta) / theta_step) + 1;
	// If the distance between the first angle and the last angle is approximately equal to pi, then the last angle will be removed
	// in order to prevent a line to be detected twice.
	if (numangle > 1 && std::fabs(std::numbers::pi - (numangle - 1) * theta_step) < theta_step / 2)
	{
		--numangle;
	}
	int numrho = std::floor((max_rho - min_rho) / rho);

		
}
