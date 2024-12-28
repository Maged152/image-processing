#include "DBSCAN.hpp"
#include <queue>

namespace qlm
{
    template <ImageFormat frmt, pixel_t T>
    Image<ImageFormat::GRAY, T> DBSCAN(const Image<frmt, T>& in, const int eps, const int min_pts)
    {
        using dist_t = qlm::wider_t<qlm::signed_t<T>>;
        const int num_points = in.width * in.height;
        std::vector<int> labels(num_points, -2); // -2 means unvisited
        int cluster_id = 0;

        // Convert image pixels to points
        std::vector<std::array<int, 5>> points(num_points);
        
        for (int y = 0; y < in.height; ++y)
        {
            for (int x = 0; x < in.width; ++x)
            {
                const auto pixel = in.GetPixel(x, y);
                points[i] = {x, y, pixel.r, pixel.g, pixel.b};
            }
        }

        // Build KD-Tree with the points
        KDTree<int, 5> kd_tree(num_points);
        kd_tree.Build(points);

        // DBSCAN algorithm
        for (int i = 0; i < num_points; ++i)
        {
            if (labels[i] != -2) // Skip visited points
            {
                continue;
            }

            // Perform radius search to find neighbors
            std::vector<int> neighbors = kd_tree.RadiusSearch(points[i], eps);

            if (neighbors.size() < min_pts)
            {
                labels[i] = -1; // Mark as noise
            }
            else
            {
                cluster_id++;
                labels[i] = cluster_id;

                std::queue<int> q;
                for (int neighbor : neighbors)
                {
                    if (neighbor != i)
                    {
                        q.push(neighbor);
                    }
                }

                while (!q.empty())
                {
                    int current = q.front();
                    q.pop();

                    if (labels[current] == -1)
                    {
                        labels[current] = cluster_id; // Change noise to border point
                    }

                    if (labels[current] != -2)
                    {
                        continue;
                    }

                    labels[current] = cluster_id;

                    std::vector<int> current_neighbors = kd_tree.RadiusSearch(points[current], eps);
                    if (current_neighbors.size() >= min_pts)
                    {
                        for (int neighbor : current_neighbors)
                        {
                            if (labels[neighbor] == -2)
                            {
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        // Create output image with cluster labels
        Image<ImageFormat::GRAY, T> out(in.width, in.height);
        for (int i = 0; i < num_points; ++i)
        {
            out.SetPixel(i, labels[i]);
        }

        return out;
    }

    template Image<ImageFormat::GRAY, uint8_t> DBSCAN<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, const int, const int);
}