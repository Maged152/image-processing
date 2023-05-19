#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Filter2D(
		const Image<frmt, T>& in,
		const Kernel& kernel,
		      Border border_type = Border::BORDER_CONSTANT,
		      int border_value = 0
	);
}