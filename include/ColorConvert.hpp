#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat src_frmt, pixel_t T, ImageFormat dst_frmt>
	Image<dst_frmt, T> ColorConvert(const Image<src_frmt, T>& in);
}