#pragma once

#include "common/types.hpp"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> EqualizeHist(
		const Image<frmt, T>& in
	);
}