#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T, typename S>
	Image<frmt, T> Translate(
		const Image<frmt, T>& in,
		const Point<S>& displacement,
		const Pixel<frmt, T>& pix = Pixel<frmt, T>{});

}