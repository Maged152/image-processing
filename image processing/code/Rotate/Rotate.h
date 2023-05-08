#pragma once

#include "types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(
		const Image<frmt, T>& in,
		      float angle,
		const Point& centre = Point{});

}