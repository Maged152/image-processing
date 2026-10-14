#include "HoughLines.hpp"
#include <iostream>

template<qlm::pixel_t T>
std::vector<qlm::LinePolar> qlm::HoughLines(const qlm::Image<qlm::ImageFormat::GRAY, T>& in, 
	const float rho, const float theta_step, const int threshold, const double min_theta, const double max_theta)
{
	const int width = in.Width();
	const int height = in.Height();

	const int max_rho = std::floor(std::sqrt(width * width + height * height));
	const int min_rho = -max_rho;

	// output lines
	std::vector<qlm::LinePolar> lines;

	if (min_theta > max_theta)
	{
		std::cout << " Error max_theta is less than min_theta\n";
		return lines;
	}

	int num_angle = std::floor((max_theta - min_theta) / theta_step);
	// If the distance between the first angle and the last angle is approximately equal to pi, then the last angle will be removed
	// in order to prevent a line to be detected twice.
	if (num_angle > 1 && std::fabs(std::numbers::pi - num_angle * theta_step) < theta_step / 2)
	{
		--num_angle;
	}

	const int num_rho = std::floor((max_rho - min_rho) / rho);
	
	// pre-compute sin & cos table
	float* sin_table = new float [num_angle]; //use std::vector
	float* cos_table = new float [num_angle];

	for (int n = 0 ; n < num_angle; n++)
	{
		const float th = min_theta + n * theta_step;
		sin_table[n] = std::sin(th);
		cos_table[n] = std::cos(th);
	}

	// Discrete accumulator array
	qlm::Image<qlm::ImageFormat::GRAY, int32_t> acc; // x -> r, y -> theta
	acc.Create(num_rho, num_angle, 0);

	// voting
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// check if exists
			const qlm::Pixel c = in.GetPixel(x, y);
			if (c.v != 0)
			{
				// loop over all angles
				for (int t = 0; t < num_angle; t++)
				{
					// calculate r
					const int r = std::round(x * cos_table[t] + y * sin_table[t]);
					
					auto val = acc.GetPixel(r, t);
					acc.SetPixel(r, t, val.v + 1);
					// acc[n * num_rho + r]++;
				}
			}
		}
	}

	
	// find lines
	double scale = 1. / (num_rho + 2);
	for (int t = 0; t < acc.Height(); t++)
	{
		for (int r = 0; r < acc.Width(); r++)
		{
			const int c_pix = acc.GetPixel(r, t).v;

			// neighboring elements
			const int l_pix = acc.GetPixel(r - 1, t).v;
			const int r_pix = acc.GetPixel(r + 1, t).v;
			const int t_pix = acc.GetPixel(r, t - 1).v;
			const int b_pix = acc.GetPixel(r, t + 1).v;

			if (c_pix > threshold && 
				c_pix > l_pix && c_pix >= r_pix &&
				c_pix > t_pix && c_pix >= b_pix)
			{
				const float theta = min_theta + t * theta_step;
				const float d  = min_rho + r * rho;

				lines.push_back({d ,theta});
			}
			
		}
	}
	
	// free the tables
	delete[] sin_table;
	delete[] cos_table;

	return lines;
}

// Explicit instantiation for GRAY , uint8_t
template std::vector<qlm::LinePolar>
qlm::HoughLines<uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
						 const float, const float, const int, const double, const double);
