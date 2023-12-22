#include "KMeans\KMeans.h"
#include <random>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(const Image<frmt, T>& in, const int k, const int max_iter, const Pixel<frmt, T> tol)
	{
		std::vector<Cluster<frmt, T>> clusters{ k };
		// initialize centroids using KMeans++
		// choose first centroid random
		std::random_device rd;  
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distribution(0, (in.Width() * in.Height)  - 1);
		int random_num = distribution(eng);

		clusters[0].color = in.GetPixel(random_num);

		for (int i = 1; i < k; i++)
		{
			Pixel<frmt, T> max_distance{};
			for (int p = 0; p < in.Width() * in.Height; p++)
			{
				// distance = || x - c||^2
				auto distance = 
			}
		}

		return clusters;
	}
}