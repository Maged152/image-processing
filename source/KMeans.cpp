#include "KMeans.hpp"
#include <random>
#include <numeric>
#include <cmath>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(const Image<frmt, T>& in, const size_t k, const KMeansInit init, const int seed, const int max_iter, const T tol)
	{
		using dist_t = qlm::wider_t<qlm::signed_t<T>>;

		std::vector<Cluster<frmt, T>> clusters{ k };

		// choose first centroid random
		std::mt19937 eng(seed);
		std::uniform_int_distribution<> distribution(0, in.width * in.height - 1);

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
			// KMeans++ initialization
			// Calculate distances to nearest centroids for remaining iterations
			const size_t num_points{ in.width * in.height };
			std::vector<dist_t> distances(num_points);

			for (int i = 1; i < k; i++)
			{
				dist_t total_weight{ 0 };
				// Update distances to nearest centroids
				for (int j = 0; j < num_points; j++)
				{
					dist_t min_dist{ std::numeric_limits<dist_t>::max() };
					for (int c = 0; c < i; c++)
					{
						auto dist = L2Norm(in.GetPixel(j), clusters[c].color);
						min_dist = std::min(min_dist, dist);
					}
					distances[j] = min_dist;
					total_weight += min_dist;
				}

				// Sample a data point based on probability distribution
				std::uniform_real_distribution<dist_t> dis_prob(0, total_weight);
				dist_t random_value = dis_prob(eng);
				int next_centroid_index = std::lower_bound(distances.begin(), distances.end(), random_value) - distances.begin();

				clusters[i].color = in.GetPixel(next_centroid_index);
			}
		}
		
		// temp memory to hold cluster information relater to each pixel
		std::vector<int> pix_cluster(in.width * in.height);
		std::vector<int> num_pix_cluster (k);
		std::vector<Pixel<frmt, float>> pix_avg (k);

		KDTree<T, 3> kd_tree(k);
		std::vector<std::array<T, 3>> centroids (k);

		for (int t = 0; t < max_iter; t++)
		{
			// reset buffers
			for (int i = 0; i < k; i++)
			{
				num_pix_cluster[i] = 0;
				pix_avg[i].Set(0.0f);

				centroids[i][0]	= clusters[i].color.r;
				centroids[i][1]	= clusters[i].color.g;
				centroids[i][2]	= clusters[i].color.b;
			}

            kd_tree.Build(centroids);

			// Assign points to the one of the K centroids
			for (int i = 0; i < in.height * in.width; i++)
			{
				const auto in_pix = in.GetPixel(i);
				const auto cluster_idx = kd_tree.NearestPoint({in_pix.r, in_pix.g, in_pix.b});

				// assign pixel to the cluster
				pix_cluster[i] = cluster_idx;
				num_pix_cluster[cluster_idx]++;

				// average pixels
				pix_avg[cluster_idx] = pix_avg[cluster_idx] + in_pix;
			}

			T centers_squared_diff {0};
		
			// Recompute the centroids
			for (int c = 0; c < k; c++)
			{
				Pixel<frmt, float> num_pix{ (float)num_pix_cluster[c] };
				Pixel<frmt, T> new_centroid = pix_avg[c] / num_pix;

				centers_squared_diff += L2Norm(new_centroid, clusters[c].color);
				clusters[c].color = new_centroid;
			}

			// early stopping
			if (centers_squared_diff <= tol)
			{
				break;
			}
		}

		const int image_width = (int)in.width;
		// populate output clusters
		for (int i = 0; i < in.height * in.width; i++)
		{
			auto cluster_idx = pix_cluster[i];
			clusters[cluster_idx].pixels.push_back({ i % image_width, i / image_width });
		}

		return clusters;
	}

	template std::vector<Cluster<ImageFormat::RGB, uint8_t>>
	KMeans<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
		const size_t, const KMeansInit, const int, const int, const uint8_t);


}