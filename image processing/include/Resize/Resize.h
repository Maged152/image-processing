#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Resize(
		const Image<frmt, T>& in,
		const unsigned int width,
		const unsigned int height,
		      InterpolationFlag method = InterpolationFlag::BILINEAR);
}