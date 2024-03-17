#include "KMeans\KMeans.h"
#include <random>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(const Image<frmt, T>& in, const unsigned int k, const KMeansInit init, const int max_iter, const Pixel<frmt, T> tol)
	{
		std::vector<Cluster<frmt, T>> clusters{ k };

		// choose first centroid random
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distribution(0, in.Width() * in.Height() - 1);

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
			//for (int i = 1; i < k; i++)
			//{
			//	Pixel<frmt, T> max_distance{};
			//	for (int p = 0; p < in.Width() * in.Height; p++)
			//	{
			//		// distance = || x - c||^2
			//		//auto distance =
			//	}
			//}

		}
		
	

		for (int t = 0; t < max_iter; t++)
		{
			// Assign points to the one of the K centroids
			for (int y = 0; y < in.Height(); y++)
			{
				for (int x = 0; x < in.Width(); x++)
				{
					auto in_pix = in.GetPixel(x, y);

					int cluster_idx{ 0 };
					uint64_t distance{ std::numeric_limits<uint64_t>::max()};

					// loop over all clusters and choose the closest
					for (int c = 0; c < k; c++)
					{
						// calculate distance
						auto cur_dist = L2Norm(clusters[c].color, in_pix);
						if (cur_dist < distance)
						{
							distance = cur_dist;
							cluster_idx = c;
						}
					}

					// assign pixel to the cluster
					clusters[cluster_idx].pixels.push_back({ x, y });

				}
			}

			// Recompute the centroids
			for (int c = 0; c < k; c++)
			{
				Pixel<frmt, float> avg{0};
				Pixel<frmt, float> num_pix{ (float)clusters[c].pixels.size() };

				for (int i = 0; i < clusters[c].pixels.size(); i++)
				{
					auto idx = clusters[c].pixels[i];
					avg = avg + in.GetPixel(idx.x, idx.y);
				}

				clusters[c].color = avg / num_pix;

				if (t != max_iter - 1)
				{
					clusters[c].pixels.clear();
				}
			}
 
		}

		return clusters;
	}

	template std::vector<Cluster<ImageFormat::RGB, uint8_t>>
	KMeans<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
		const unsigned int, const KMeansInit, const int, const Pixel<ImageFormat::RGB, uint8_t>);


}