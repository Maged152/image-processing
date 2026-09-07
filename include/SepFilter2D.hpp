#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	void SepFilter2D(
		const Image<frmt, src_t>& in,
		      Image<frmt, dst_t>& out,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode = BorderMode<frmt, src_t>{},
		const Rectangle<int>& roi = Rectangle<int>{});

	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> SepFilter2D(
		const Image<frmt, src_t>& in,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode =  BorderMode<frmt, src_t>{},
		const Rectangle<int>& roi = Rectangle<int>{});

}
