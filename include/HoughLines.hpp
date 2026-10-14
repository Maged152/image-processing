#pragma once

#include "common/types.hpp"
#include <numbers>
#include <vector>


namespace qlm
{
	template<pixel_t T>
	std::vector<LinePolar> HoughLines(const Image<ImageFormat::GRAY, T>& in,
									  const float  rho, const float theta_step, const int threshold,
									  const double min_theta = 0, const double max_theta = std::numbers::pi);
}