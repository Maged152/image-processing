#pragma once

#include "common/types.hpp"

namespace qlm
{
	struct DBSCANResult
    {
        Image<ImageFormat::GRAY, int> labels;
        int num_clusters;
        bool noise_exists;
    };
	
    template<ImageFormat frmt, pixel_t T>
	DBSCANResult DBSCAN(
		const Image<frmt, T>& in,
		const int eps,
		const int min_pts = 5
	);
}