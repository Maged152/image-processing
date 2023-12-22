#pragma once
#include "common/types.h"
#include <vector>


namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(
		const Image<frmt, T>& in,
		const int k = 8,
		const int max_iter = 300,
		const Pixel<frmt, T> tol = Pixel<frmt, T>{}); // flag to include distance
}