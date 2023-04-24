#pragma once

#include "types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SepFilter2D(
		const Image<frmt, T>& in,
		const Kernel1D& x_kernel,
		const Kernel1D& y_kernel,
		const Border border_type = Border::BORDER_CONSTANT,
		const int border_value = 0
	);

}
