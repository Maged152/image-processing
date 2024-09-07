#pragma once

#include "common/types.hpp"

namespace qlm
{
	// generate gaussian kernel
	Kernel1D GetGaussianKernel(const size_t filter_size, float sigma);
	
	// gaussian filter
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Gaussian(
		const Image<frmt, T>& in,
		const size_t filter_size,
		float sigma_x,
		float sigma_y = 0,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);

}