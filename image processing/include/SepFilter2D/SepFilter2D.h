#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> SepFilter2D(
		const Image<frmt, src_t>& in,
		const Kernel1D& x_kernel,
		const Kernel1D& y_kernel,
		const BorderMode& border_mode = BorderMode{}
	);

}
