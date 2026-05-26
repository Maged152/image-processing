#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Canny(
		const Image<frmt, T>& in,
		const int threshold_low, const int threshold_high,
		const int filter_size = 3,
		const bool l2_gradient = false ,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);

}