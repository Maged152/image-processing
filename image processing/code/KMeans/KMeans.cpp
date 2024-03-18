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
		
		// temp memory to hold cluster information relater to each pixel
		int* pix_cluser = new int[in.Width() * in.Height()];
		int* num_pix_cluster = new int[k];
		Pixel<frmt, float>* pix_avg = new Pixel<frmt, float>[k];
	

		for (int t = 0; t < max_iter; t++)
		{
			// reset buffers
			for (int i = 0; i < k; i++)
			{
				num_pix_cluster[i] = 0;
				pix_avg[i].Set(0.0f);
			}

			// Assign points to the one of the K centroids
			for (int i = 0; i < in.Height() * in.Width(); i++)
			{
				auto in_pix = in.GetPixel(i);

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
				pix_cluser[i] = cluster_idx;
				num_pix_cluster[cluster_idx]++;
			}

			// Recompute the centroids
			for (int i = 0; i < in.Height() * in.Width(); i++)
			{
				auto pix = in.GetPixel(i);
				auto cluster_idx = pix_cluser[i];
				pix_avg[cluster_idx] = pix_avg[cluster_idx] + pix;
			}

			for (int c = 0; c < k; c++)
			{
				Pixel<frmt, float> num_pix{ (float)num_pix_cluster[c]};
				clusters[c].color = pix_avg[c] / num_pix;
			}
		}

		const int image_width = (int)in.Width();
		// populate output clusters
		for (int i = 0; i < in.Height() * in.Width(); i++)
		{
			auto cluster_idx = pix_cluser[i];
			clusters[cluster_idx].pixels.push_back({ i % image_width, i / image_width });
		}

		delete[] pix_cluser;
		delete[] num_pix_cluster;
		delete[] pix_avg;

		return clusters;
	}

	template std::vector<Cluster<ImageFormat::RGB, uint8_t>>
	KMeans<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
		const unsigned int, const KMeansInit, const int, const Pixel<ImageFormat::RGB, uint8_t>);


}