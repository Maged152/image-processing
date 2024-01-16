#pragma once

#include "common/types.h"

namespace qlm
{
	// box filter
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> BoxFilter(
		const Image<frmt, T>& in,
		const unsigned int filter_x_size,
		const unsigned int filter_y_size,
		const bool normalize = true,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);

}