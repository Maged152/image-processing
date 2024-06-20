#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> SepFilter2D(
		const Image<frmt, src_t>& in,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode =  BorderMode<frmt, src_t>{}
	);

}
