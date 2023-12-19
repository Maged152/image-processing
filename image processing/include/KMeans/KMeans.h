#pragma once
#include "common/types.h"
#include <vector>


namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(
		const Image<frmt, T>& in,
		const int k,
		const int max_iter,
		const Pixel<frmt, T> tol); // flag to initialization
}