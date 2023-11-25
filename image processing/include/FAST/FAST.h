#pragma once

#include "common/types.h"
#include <vector>


namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<KeyPoint<int>> FAST(
		const Image<frmt, T>& in,
		int arc_length = 9,
		int threshold = 10,
		bool non_max_suppression = true);
}