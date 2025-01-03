#include "DBSCAN.hpp"

namespace qlm
{

    template <ImageFormat frmt, pixel_t T>
    std::vector<std::array<int, 5>> ConvertImageToPoints(const Image<frmt, T>& in)
    {
        constexpr bool insert_row = true;
        constexpr bool insert_col = false;

        std::vector<std::array<int, 5>> points (in.width * in.height);

        const auto InsertBalanced = []<bool x_axis>(std::vector<int>& points, const int start, const int end, int& index) 
        {
            if (start >= end)
                return;

            const int mid = (start + end) / 2;

            if constexpr (x_axis)
            {
                const auto pixel = in.GetPixel(mid, 0);
                points[index] = {mid, 0, pixel.r, pixel.g, pixel.b};
            }
            else
            {
                const auto pixel = in.GetPixel(0, mid);
                points[index] = {0, mid, pixel.r, pixel.g, pixel.b};
            }

            index += 5;
            InsertBalanced<x_axis>(points, mid + 1, end, index);  // Right half

            index += 5;
            InsertBalanced<x_axis>(points, start, mid, index);  // Left half
        };
        
        const bool width_bigger = in.width > in.height;
        int index;

        if (width_bigger) 
        {
            index = 1;
            insertBalanced<insert_col>(points, 0, in.height, index);

            index = 0;
            insertBalanced<insert_row>(points, 0, in.width, index);
        }
        else
        {
            index = 0;
            insertBalanced<insert_col>(points, 0, in.height, index);

            index = 1;
            insertBalanced<insert_row>(points, 0, in.width, index);
        }

        const int abs_diff = std::abs(in.width - in.height);
        const int min_dim = std::min(in.width, in.height);
        

        // fill difference in dimensions

        
        

        int start_x {1}, start_y {1};


        for (int y = 1; y < in.height; ++y)
        {
            for (int x = 1; x < in.width; ++x)
            {
                const auto pixel = in.GetPixel(x, y);
                points[y * in.width + x] = {x, y, pixel.r, pixel.g, pixel.b};
            }
        }

        return points;
    }

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
        std::vector<std::array<int, 5>> points = ConvertImageToPoints(in);//(num_points);

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