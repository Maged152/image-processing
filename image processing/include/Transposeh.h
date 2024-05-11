#pragma once

#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(const Image<frmt, T>& in);

	template<ImageFormat frmt, pixel_t T>
	void Transpose(const Image<frmt, T>& in, Image<frmt, T>& out);

}