#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Resize(
		const Image<frmt, T>& in,
		const int width,
		const int height,
		const InterpolationFlag method = InterpolationFlag::BILINEAR,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}