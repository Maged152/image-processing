#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::pair<std::vector<Point<int>>, std::vector<Point<int>>> MinMaxLoc(
		const Image<frmt, T>& in
	);

}