#pragma once
#include "common/types.h"
#include <vector>


namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(
		const Image<frmt, T>& in,
		const unsigned int k = 8,
		const KMeansInit init = KMeansInit::RANDOM,
		const int max_iter = 300,
		const T tol = 1 );
}