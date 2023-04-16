#pragma once

#include "types.h"
#include <numbers>
#include <vector>


namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<LinePolar> HoughLines(const Image<frmt, T>& in,
									  float  rho, float theta_step, int threshold,
									  double min_theta = 0, double max_theta = std::numbers::pi);
}