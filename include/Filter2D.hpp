#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Filter2D(
		const Image<frmt, T>& in,
		const Kernel& kernel,
		const  BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);
}