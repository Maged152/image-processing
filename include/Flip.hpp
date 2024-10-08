#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Flip(
		const Image<frmt, T>& in,
		const FlipFlag Flip_code);

}