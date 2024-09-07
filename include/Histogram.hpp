#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Histogram_t<frmt, T> Histogram(
		const Image<frmt, T>& in);

}