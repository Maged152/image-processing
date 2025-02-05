#pragma once
#include "common/types.hpp"
#include <vector>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T = uint8_t>
	std::vector<KeyPoint<int>> HarrisCorner(
		const Image<frmt, T>& in,
		const int block_size,
		const int filter_size,
		float k,
		float threshold,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);
}