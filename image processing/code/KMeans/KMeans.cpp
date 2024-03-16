#include "KMeans\KMeans.h"
#include <random>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(const Image<frmt, T>& in, const int k, const KMeansInit init, const int max_iter, const Pixel<frmt, T> tol)
	{
		std::vector<Cluster<frmt, T>> clusters{ k };

		// choose first centroid random
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distribution(0, (in.Width() * in.Height) - 1);

		clusters[0].color = in.GetPixel(distribution(eng));

		if (init == KMeansInit::RANDOM)
		{
			// random initialization for all centroids
			for (int i = 1; i < k; i++)
			{
				clusters[i].color = in.GetPixel(distribution(eng));
			}
		}
		else
		{
			// initialize centroids using KMeans++
			for (int i = 1; i < k; i++)
			{
				Pixel<frmt, T> max_distance{};
				for (int p = 0; p < in.Width() * in.Height; p++)
				{
					// distance = || x - c||^2
					//auto distance =
				}
			}

		}
		
		const int num_pix = in.Width() * in.Height;

		for (int t = 0; t < max_iter; t++)
		{
			for (int p = 0; p < num_pix; p++)
			{
				int centroid_idx{ 0 };
				uint64_t distance{ std::numeric_limits<uint64_t>::max };
				for (int c = 0; c < k; c++)
				{
					// calculate distance
					auto cur_dist = L2Norm(clusters[i].color, in.GetPixel(p));
					if (cur_dist < distance)
					{
						distance = cur_dist;
						centroid_idx = c;
					}
				}
			}
		}

		return clusters;
	}
}