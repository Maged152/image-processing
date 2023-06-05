#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> FloodFill(
		const Image<frmt, T>& in,
		const Point<int>& seed_point,
		const Pixel<frmt, T>& fill_color,
		const Pixel<frmt, T>& threshold = Pixel<frmt, T>{});
}