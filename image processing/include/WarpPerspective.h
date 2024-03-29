#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpPerspective(
		const Image<frmt, T>& in,
		const PerspectiveMatrix& mat,
		const int dst_width,
		const int dst_height,
		const InterpolationFlag inter,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});

	PerspectiveMatrix GetPerspectiveTransform(const Point<int> src[4], const Point<int> dst[4]);
}