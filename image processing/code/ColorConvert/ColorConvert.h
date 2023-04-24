#pragma once

#include "types.h"

namespace qlm
{
	template<ImageFormat src_frmt, pixel_t src_t, ImageFormat dst_frmt, pixel_t dst_t>
	Image<dst_frmt, dst_t> ColorConvert(const Image<src_frmt, src_t>& in);
}