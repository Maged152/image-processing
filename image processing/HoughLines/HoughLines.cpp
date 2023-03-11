#include "HoughLines.h"
#include <iostream>


void qlm::HoughLines(const sf::Image& in, std::vector<LinePolar>& lines, float rho, float theta_step, int threshold, double min_theta, double max_theta)
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
	// sin & cos table
	float* sin_table = new float [numangle];
	float* cos_table = new float [numangle];
	// loop over all angles
	for (int n = 0 ; n < numangle; n++)
	{
		float th = min_theta;
		th += n * theta_step;
		sin_table[n] = std::sin(th);
		cos_table[n] = std::cos(th);
	}
	// accumulator
	int* acc = new int[numangle * numrho]; // acc[r][th] => acc[th * numrho + r]
	// fill the accumulator
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// check if exists
			sf::Color c = in.getPixel(x, y);
			if (c.r != 0)
			{
				// loop over all angles
				for (int n = 0; n < numangle; n++)
				{
					// calculate r
					int r = std::round(x * sin_table[n] + y * cos_table[n]);
					acc[n * numrho + r]++;
				}
			}
		}
	}
	// free the tables
	delete[] sin_table;
	delete[] cos_table;
	// find lines
	for (int r = 0; r < numrho; r++)
	{
		for (int t = 0; t < numrho; t++)
		{
			int idx = t * numrho + r;
			int idx_p = std::max(idx - 1, 0);
			int idx_n = std::min(idx + 1, numangle * numrho);
			if (acc[idx] > threshold && acc[idx] > acc[idx_p] && acc[idx] >= acc[idx_n])
			{
				float theta = min_theta + t * theta_step;
				float slope = (r - (numrho - 1) * 0.5f) * rho;
				lines.push_back({slope ,theta});
			}
			
		}
	}
	// free accumulator
	delete[] acc;
}
