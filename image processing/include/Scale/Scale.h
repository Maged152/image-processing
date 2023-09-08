#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Scale(const Image<frmt, T>& in,
							   InterpolationFlag method,
		                       float scale_x, 
		                       float scale_y);
} // namespace