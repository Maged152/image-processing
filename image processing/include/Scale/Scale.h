#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Scale(const Image<frmt, T>& in,
						 const InterpolationFlag method,
		                 const float scale_x, 
		                 const float scale_y,
						 const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
} 