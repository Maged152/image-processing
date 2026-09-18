#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat dst_frmt, ImageFormat src_frmt, pixel_t T>
	Image<dst_frmt, T> ColorConvert(const Image<src_frmt, T>& in);
}