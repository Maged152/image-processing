#include "HoughLines.hpp"
#include <iostream>

template<qlm::ImageFormat frmt, qlm::pixel_t T>
std::vector<qlm::LinePolar> qlm::HoughLines(const qlm::Image<frmt, T>& in, float rho, float theta_step, int threshold, double min_theta, double max_theta)
{
	// TODO: check if not gray convert to gray

	int width = in.width;
	int height = in.height;

	int max_rho = std::floor(std::sqrt(width * width + height * height));
	int min_rho = 0; //-max_rho
	// output lines
	std::vector<qlm::LinePolar> lines;

	if (min_theta > max_theta)
	{
		std::cout << " Error max_theta is less than min_theta\n";
		return lines;
	}
	int numangle = std::floor((max_theta - min_theta) / theta_step);
	// If the distance between the first angle and the last angle is approximately equal to pi, then the last angle will be removed
	// in order to prevent a line to be detected twice.
	if (numangle > 1 && std::fabs(std::numbers::pi - numangle * theta_step) < theta_step / 2)
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
		float th = min_theta + n * theta_step;
		sin_table[n] = std::sin(th);
		cos_table[n] = std::cos(th);
	}
	// accumulator
	int* acc = new int[numangle * numrho]; // acc[r][th] => acc[th * numrho + r]
	std::memset(acc, 0, static_cast<unsigned long long>(numangle) * numrho * sizeof(int));
	// fill the accumulator
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// check if exists
			qlm::Pixel c = in.GetPixel(x, y);
			if (c.v != 0)
			{
				// loop over all angles
				for (int n = 0; n < numangle; n++)
				{
					// calculate r
					int r = std::round(x * cos_table[n] + y * sin_table[n]);
					acc[n * numrho + r]++;
				}
			}
		}
	}
	// free the tables
	delete[] sin_table;
	delete[] cos_table;
	// find lines
	double scale = 1. / (numrho + 2);
	for (int r = 0; r < numrho; r++)
	{
		for (int t = 0; t < numangle; t++)
		{
			int idx = t * numrho + r;
			// neighboring elements
			int idx_l = std::max(idx - 1, 0);                          // left
			int idx_r = std::min(idx + 1, numangle * numrho - 1);      // right
			int idx_a = std::max(idx - numrho, 0);                     // above
			int idx_b = std::min(idx + numrho, numangle * numrho - 1); // below
			if (acc[idx] > threshold && 
				acc[idx] > acc[idx_l] && acc[idx] >= acc[idx_r] &&
				acc[idx] > acc[idx_a] && acc[idx] >= acc[idx_b])
			{
				float theta = min_theta + t * theta_step;
				float d  = min_rho + r * rho;;
					//min_rho + r * rho;// (r - numrho * 0.5f)* rho;
				lines.push_back({d ,theta});
			}
			
		}
	}
	// free accumulator
	delete[] acc;

	return lines;
}

// Explicit instantiation for GRAY , uint8_t
template std::vector<qlm::LinePolar>
qlm::HoughLines<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
												 float, float, int, double, double);
// Explicit instantiation for GRAY , int16_t
template std::vector<qlm::LinePolar>
qlm::HoughLines<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	                                             float, float, int, double, double);