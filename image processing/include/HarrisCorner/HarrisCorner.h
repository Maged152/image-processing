#pragma once
#include "common/types.h"
#include <vector>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T = uint8_t>
	std::vector<KeyPoint<int>> HarrisCorner(
		const Image<frmt, T>& in,
		const unsigned int block_size,
		const unsigned int filter_size,
		float k,
		float threshold,
		const Border border_type = Border::BORDER_CONSTANT,
		const int border_value = 0
	);
}