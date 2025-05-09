#pragma once

#include "common/types.hpp"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
	DBSCANResult DBSCAN(
		const Image<frmt, T>& in,
		const int eps,
		const int min_pts = 5
	);
}