#include "DBSCAN.hpp"
#include <queue>

namespace qlm
{
    template <ImageFormat frmt, pixel_t T>
    DBSCANResult DBSCAN(const Image<frmt, T>& in, const int eps, const int min_pts)
    {
        const int num_points = in.width * in.height;
        const Pixel<ImageFormat::GRAY, int> unvisited{-2};

        DBSCANResult result{};
        result.labels.create(in.width, in.height, unvisited); // -2 means unvisited
        result.num_clusters = -1;

        int num_noise_pixels = 0;
       
        // Convert image pixels to points
        std::vector<std::array<int, 5>> points(num_points);
        
        for (int y = 0; y < in.height; ++y)
        {
            for (int x = 0; x < in.width; ++x)
            {
                const auto pixel = in.GetPixel(x, y);
                points[y * in.width + x] = {x, y, pixel.r, pixel.g, pixel.b};
            }
        }

        // Build KD-Tree with the points
        KDTree<int, 5> kd_tree(num_points);
        kd_tree.Build(points);

        // DBSCAN algorithm
        for (int i = 0; i < num_points; i++)
        {
            if (result.labels.GetPixel(i).v != -2) // Skip visited points
            {
                continue;
            }

            // Perform radius search to find neighbors
            std::vector<int> neighbors = kd_tree.RadiusSearch(points[i], eps);

            if (neighbors.size() < min_pts)
            {
                result.labels.SetPixel(i, -1); // Mark as noise
                num_noise_pixels++;
            }
            else
            {
                result.num_clusters++;
                result.labels.SetPixel(i, result.num_clusters);

                std::queue<int> q;
                for (int neighbor : neighbors)
                {
                    q.push(neighbor);
                }

                while (!q.empty())
                {
                    int current = q.front();
                    q.pop();

                    if (result.labels.GetPixel(current).v == -1)
                    {
                        result.labels.SetPixel(current, result.num_clusters); // Change noise to border point
                        num_noise_pixels--;
                    }

                    if (result.labels.GetPixel(current).v != -2)
                    {
                        continue;
                    }

                    result.labels.SetPixel(current, result.num_clusters);

                    std::vector<int> current_neighbors = kd_tree.RadiusSearch(points[current], eps);
                    if (current_neighbors.size() >= min_pts)
                    {
                        for (int neighbor : current_neighbors)
                        {
                            if (result.labels.GetPixel(neighbor).v == -2)
                            {
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        if (num_noise_pixels > 0)
        {
            result.noise_exists = true;
        }

        result.num_clusters++; // Increment by 1 to convert to the actual number of clusters
        return result;
    }

    template DBSCANResult DBSCAN<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, const int, const int);
}