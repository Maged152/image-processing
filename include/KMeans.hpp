#pragma once
#include "common/types.hpp"
#include <vector>


namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(
		const Image<frmt, T>& in,
		const int k = 8,
		const KMeansInit init = KMeansInit::KMEANS_PLUS_PLUS,
		const int seed = 0,
		const int max_iter = 300,
		const T tol = 0);
}