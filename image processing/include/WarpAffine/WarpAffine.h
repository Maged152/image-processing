#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(
		const Image<frmt, T>& in,
		const TransformationMatrix& mat,
		const int dst_width,
		const int dst_height,
		const InterpolationFlag inter,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});

	TransformationMatrix GetRotationMatrix(const Point<int>& center, const float angle, const float scale);

	TransformationMatrix GetAffineTransform(const Point<int> src[3], const Point<int> dst[3]);

}