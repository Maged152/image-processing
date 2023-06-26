#pragma once

#include "common/types.h"

namespace qlm
{
	// generate gaussian kernel
	Kernel1D GetGaussianKernel(unsigned int filter_size, double sigma);
	
	// gaussian filter
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Gaussian(
		const Image<frmt, T>& in,
		unsigned int filter_size,
		double 	sigma_x,
		double 	sigma_y = 0
		Border border_type = Border::BORDER_CONSTANT,
		int border_value = 0
	);

}