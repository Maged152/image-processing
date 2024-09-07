#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(
		const Image<frmt, T>& in,
		const AffineMatrix& mat,
		const int dst_width,
		const int dst_height,
		const InterpolationFlag inter,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});

	AffineMatrix GetRotationMatrix(const Point<int>& center, const float angle, const float scale);

	AffineMatrix GetAffineTransform(const Point<int> src[3], const Point<int> dst[3]);

}