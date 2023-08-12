#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Filter2D(
		const Image<frmt, T>& in,
		const Kernel& kernel,
		const BorderMode& border_mode = BorderMode{}
	);
}