#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(
		const Image<frmt, T>& in,
		const RotateFlag rotate_code);

}