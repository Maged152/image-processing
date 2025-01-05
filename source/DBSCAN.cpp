#include "DBSCAN.hpp"

namespace qlm
{

    template <ImageFormat frmt, pixel_t T>
    std::vector<std::array<int, 5>> ConvertImageToPoints(const Image<frmt, T>& in)
    {
        constexpr bool insert_row = true;
        constexpr bool insert_col = false;

        std::vector<std::array<int, 5>> points (in.width * in.height);

        const auto InsertBalanced = []<bool x_axis>(std::vector<int>& points, const int start, const int end, int& index, int& c_index) 
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

                const auto pixel_r = in.GetPixel(1, c_index);
                const auto pixel_g = in.GetPixel(2, c_index);
                const auto pixel_b = in.GetPixel(3, c_index);

                points[index + 1] = {1, c_index, pixel_r.r, pixel_r.g, pixel_r.b};
                points[index + 2] = {2, c_index, pixel_g.r, pixel_g.g, pixel_g.b};
                points[index + 3] = {3, c_index, pixel_b.r, pixel_b.g, pixel_b.b};

                c_index++;
            }

            index += 5;
            InsertBalanced<x_axis>(points, mid + 1, end, index, c_index);  // Right half

            index += 5;
            InsertBalanced<x_axis>(points, start, mid, index, c_index);  // Left half
        };
        
        const bool width_bigger = in.width > in.height;
        int index = 1, c_index = 0;

        insertBalanced<insert_col>(points, 0, in.height, index, c_index);

        index = 0;
        insertBalanced<insert_row>(points, 0, in.width, index, c_index);
        
        // fix double insertion
        const auto pixel_0 = in.GetPixel(4, 1);
        const auto pixel_1 = in.GetPixel(5, 1);
        const auto pixel_2 = in.GetPixel(6, 1);
        const auto pixel_3 = in.GetPixel(7, 1);

        points[2] = {4, 1, pixel_0.r, pixel_0.g, pixel_0.b};
        points[3] = {5, 1, pixel_1.r, pixel_1.g, pixel_1.b};
        points[4] = {6, 1, pixel_2.r, pixel_2.g, pixel_2.b};
        points[in.width * 5] = {7, 1, pixel_3.r, pixel_3.g, pixel_3.b};

        int start_x = 7, start_y = 1;

        // fill difference in dimensions
        const int abs_diff = std::abs(in.width - in.height);
        const int min_dim = std::min(in.width, in.height);

        const int remaining = width_bigger ? abs_diff * 4 : abs_diff;

        int start_idx = min_dim * 5;

        for (int y = start_y; y < in.height; ++y)
        {
            for (int x = start_x; x < in.width; ++x)
            {
                const auto pixel = in.GetPixel(x, y);
                
                if (remaining > 0)
                {
                    
                }
                else 
                {

                }
                points[y * in.width + x] = {x, y, pixel.r, pixel.g, pixel.b};

                remaining--;
            }

            start_x = 4;
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
        std::vector<std::array<int, 5>> points = ConvertImageToPoints(in);

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