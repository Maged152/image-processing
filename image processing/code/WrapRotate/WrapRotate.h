#pragma once

#include "types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WrapRotate(
		const Image<frmt, T>& in,
		      float angle,
		const Point<int>& centre,
		const float scale = 1,
		const Pixel<frmt, T>& pix = Pixel<frmt, T>{});

}