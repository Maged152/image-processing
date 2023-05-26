#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Translate(
		const Image<frmt, T>& in,
		const Point<int>& displacement,
		const Pixel<frmt, T>& pix = Pixel<frmt, T>{});

}