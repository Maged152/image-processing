#pragma once

#include "common/types.hpp"

namespace qlm
{
	// box filter
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> BoxFilter(
		const Image<frmt, src_t>& in,
		const size_t filter_x_size,
		const size_t filter_y_size,
		const bool normalize = true,
		const BorderMode<frmt, src_t>& border_mode = BorderMode<frmt, src_t>{}
	);

}